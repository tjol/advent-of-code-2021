#!/usr/bin/env node

'use strict';

async function readAll(stream) {
    const chunks = []
    for await (const chunk of stream) chunks.push(chunk);
    return Buffer.concat(chunks).toString('utf8')
}

class Instruction {
    constructor(line) {
        let [cmd, volStr] = line.split(' ', 2)
        this.command = cmd
        this.volume = new Cuboid(volStr)
    }
}

class Cuboid {
    constructor(...args) {
        if (args.length == 1) {
            const stringForm = args[0]
            let [xStr, yStr, zStr] = stringForm.split(',')
            const exp = /^[xyz]=([-0-9]+)\.\.([-0-9]+)$/
            this.xlim = new Segment(...exp.exec(xStr).slice(1).map(s => parseInt(s)))
            this.ylim = new Segment(...exp.exec(yStr).slice(1).map(s => parseInt(s)))
            this.zlim = new Segment(...exp.exec(zStr).slice(1).map(s => parseInt(s)))
        } else if (args.length == 3) {
            this.xlim = args[0]
            this.ylim = args[1]
            this.zlim = args[2]
        } else {
            throw "PANIC"
        }
    }

    intersection(otherCuboid) {
        let c = new Cuboid(
            this.xlim.intersection(otherCuboid.xlim),
            this.ylim.intersection(otherCuboid.ylim),
            this.zlim.intersection(otherCuboid.zlim))
        return c
    }

    get volume() {
        return this.xlim.length * this.ylim.length * this.zlim.length
    }

    equalTo(otherCuboid) {
        return this.xlim.equalTo(otherCuboid.xlim)
            && this.ylim.equalTo(otherCuboid.ylim)
            && this.zlim.equalTo(otherCuboid.zlim)
    }

    toString() {
        return `x=${this.xlim.toString()},y=${this.ylim.toString()},z=${this.zlim.toString()}`
    }
}

class Segment {
    constructor(start, end) {
        if (start > end) {
            this.start = 0
            this.end = -1
        } else {
            this.start = start
            this.end = end
        }
    }

    intersection(other) {
        const maxstart = Math.max(this.start, other.start)
        const minend = Math.max(Math.min(this.end, other.end), maxstart - 1)
        return new Segment(maxstart, minend)
    }

    get length() {
        return this.end - this.start + 1
    }

    equalTo(other) {
        return this.start == other.start && this.end == other.end
    }

    toString() {
        return this.length == 0 ? "ø" : `${this.start}..${this.end}`
    }
}

function doInitProc(instructions) {
    let volumes = [];
    for (const instr of instructions) {
        let newVolumeSegments = {};
        let removeVolumeIndices = [];
        const newVol = instr.volume
        volumes.forEach((elem, idx) => {
            const [val, oldVol] = elem
            const inCommon = newVol.intersection(oldVol)
            if (inCommon.equalTo(oldVol)) {
                removeVolumeIndices.push(idx)
            } else if (inCommon.volume != 0) {
                // neutralize any other cuboids in "our" volume region
                const inCommonStr = inCommon.toString()
                newVolumeSegments[inCommonStr] = (newVolumeSegments[inCommonStr] ?? 0) - val
            }
        })
        for (const idx of removeVolumeIndices.reverse()) {
            volumes.splice(idx, 1)
        }
        for (const [volStr, value] of Object.entries(newVolumeSegments)) {
            if (value != 0) {
                volumes.push([value, new Cuboid(volStr)]);
            }
        }
        if (instr.command == 'on') {
            volumes.push([1, newVol])
        }
    }
    return volumes
}

function countLitVoxels(volList) {
    let count = 0
    for (const [weight, vol] of volList) {
        // console.log(`Applying value ${weight} to volume ${vol.toString()} [${vol.volume}]`)
        count += weight * vol.volume
    }
    return count
}

function parse(input) {
    const instructions = input.split('\n')
        .map(l => l.trim())
        .filter(l => !!l)
        .map(l => new Instruction(l))

    const volList = doInitProc(instructions)

    console.log(countLitVoxels(volList))
}

readAll(process.stdin).then(parse)