use std::io::{stdin, BufRead};
use std::str::FromStr;
use std::iter::repeat;

#[derive(Debug, Clone)]
struct Bits {
    data: Vec<u8>,
    pos: usize,
}

impl FromStr for Bits {
    type Err = ();
    fn from_str(s: &str) -> Result<Self, ()> {
        let s = s.trim();
        let first_digits = s.chars().step_by(2);
        let second_digits = s.chars().skip(1).step_by(2).chain(repeat('0'));
        let data: Vec<u8> = first_digits.zip(second_digits).map(|(a, b)|
            u8::from_str_radix(&[a, b].into_iter().collect::<String>(), 16)
        ).flatten().collect();
        if data.is_empty() {
            Err(())
        } else {
            Ok(Self { data, pos: 0 })
        }
    }
}

#[derive(Debug)]
enum PacketValue {
    Literal(u64),
    Sum(Vec<Packet>),
    Product(Vec<Packet>),
    Minimum(Vec<Packet>),
    Maximum(Vec<Packet>),
    IsGreaterThan(Box<Packet>, Box<Packet>),
    IsLessThan(Box<Packet>, Box<Packet>),
    IsEqual(Box<Packet>, Box<Packet>)
}

#[derive(Debug)]
struct Packet {
    version: u8,
    value: PacketValue
}

impl Packet {
    pub fn value(&self) -> u64 {
        match &self.value {
            PacketValue::Literal(n) => *n,
            PacketValue::Sum(packets) => packets.iter().map(|p| p.value()).sum(),
            PacketValue::Product(packets) => packets.iter().map(|p| p.value()).reduce(|a, b| a*b).unwrap(),
            PacketValue::Minimum(packets) => packets.iter().map(|p| p.value()).min().unwrap(),
            PacketValue::Maximum(packets) => packets.iter().map(|p| p.value()).max().unwrap(),
            PacketValue::IsGreaterThan(a, b) => if a.value() > b.value() { 1 } else { 0 },
            PacketValue::IsLessThan(a, b) => if a.value() < b.value() { 1 } else { 0 },
            PacketValue::IsEqual(a, b) => if a.value() == b.value() { 1 } else { 0 },
        }
    }
}

impl Bits {
    fn peek_bytes(&self) -> impl Iterator<Item=u8> + '_ {
        let byte_pos = self.pos / 8;
        let bit_pos = self.pos % 8;
        let data_slice = &self.data[byte_pos..];
        data_slice.iter().copied().zip(data_slice.iter().copied().skip(1).chain(repeat(0u8)))
            .map(move |(a, b)| {
                (a << bit_pos) | if bit_pos == 0 { 0 } else { b >> (8 - bit_pos) }
            })
    }

    pub fn peek_bits(&self, n: usize) -> Vec<u8> {
        assert!(n > 0);
        let bytes_needed = (n - 1) / 8 + 1;
        let extra_bits = n % 8;
        let rshift = if extra_bits != 0 { 8 - extra_bits } else { 0 };
        let mut bytes: Vec<u8> = self.peek_bytes().take(bytes_needed).collect();
        for i in (1..bytes_needed).rev() {
            bytes[i] = (bytes[i] >> rshift) | (bytes[i-1] << (8 - rshift));
        }
        bytes[0] >>= rshift;
        bytes
    }

    pub fn peek_bits_leftaligned(&self, n: usize) -> Vec<u8> {
        assert!(n > 0);
        let bytes_needed = (n - 1) / 8 + 1;
        let bytes: Vec<u8> = self.peek_bytes().take(bytes_needed).collect();
        bytes
    }

    pub fn take_bits(&mut self, n: usize) -> Vec<u8> {
        let result = self.peek_bits(n);
        self.pos += n;
        result
    }

    pub fn take_bits_leftaligned(&mut self, n: usize) -> Vec<u8> {
        let result = self.peek_bits_leftaligned(n);
        self.pos += n;
        result
    }

    pub fn take_bits_u32(&mut self, n: usize) -> u32 {
        assert!(n <= 32);
        let bytes = self.take_bits(n);
        let mut result = 0;
        for (i, b) in bytes.into_iter().rev().enumerate() {
            let lshift = 8 * i;
            result |= (b as u32) << lshift;
        }
        result
    }

    pub fn take_bits_u8(&mut self, n: usize) -> u8 {
        assert!(n <= 8);
        self.take_bits(n)[0]
    }


    pub fn len_remaining(&self) -> usize {
        8 * self.data.len() - self.pos
    }

    pub fn pop_packet(&mut self) -> Packet {
        // get the version
        let version = self.take_bits_u8(3);
        let type_id = self.take_bits_u8(3);
        match type_id {
            4 => {
                // Literal value
                let mut value = 0u64;
                loop {
                    let chunk = self.take_bits_u8(5);
                    value <<= 4;
                    value |= (chunk & 0xF) as u64;
                    if chunk & 0x10 == 0 {
                        break;
                    }
                }
                Packet {
                    version,
                    value: PacketValue::Literal(value)
                }
            }
            _ => {
                // Operator
                let len_type_id = self.take_bits_u8(1);
                let mut packets = vec![];
                match len_type_id {
                    0 => {
                        let bit_len = self.take_bits_u32(15);
                        let mut sub_bits = Bits { data: self.take_bits_leftaligned(bit_len as usize), pos: 0 };
                        while sub_bits.len_remaining() >= 11 { // 11 = min packet size
                            packets.push(sub_bits.pop_packet());
                        }
                    },
                    1 => {
                        let count = self.take_bits_u32(11);
                        for _ in 0..count {
                            packets.push(self.pop_packet());
                        }
                    }
                    _ => unreachable!()
                }
                let value = match type_id {
                    0 => PacketValue::Sum(packets),
                    1 => PacketValue::Product(packets),
                    2 => PacketValue::Minimum(packets),
                    3 => PacketValue::Maximum(packets),
                    5 => {
                        assert!(packets.len() == 2);
                        let b = packets.pop().unwrap();
                        let a = packets.pop().unwrap();
                        PacketValue::IsGreaterThan(Box::new(a), Box::new(b))
                    }
                    6 => {
                        assert!(packets.len() == 2);
                        let b = packets.pop().unwrap();
                        let a = packets.pop().unwrap();
                        PacketValue::IsLessThan(Box::new(a), Box::new(b))
                    }
                    7 => {
                        assert!(packets.len() == 2);
                        let b = packets.pop().unwrap();
                        let a = packets.pop().unwrap();
                        PacketValue::IsEqual(Box::new(a), Box::new(b))
                    },
                    _ => unreachable!()
                };
                Packet { version, value }
            }
        }
    }
}


fn main() {
    let mut bits: Bits = stdin().lock().lines().next().unwrap().unwrap().parse().unwrap();

    println!("{}", bits.pop_packet().value());
}
