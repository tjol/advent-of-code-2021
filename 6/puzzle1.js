#!/usr/bin/env node

const POSSIBLE_STATES = 9
const REBIRTH_AGE = 6
const BABY_AGE = 8

async function readAll(stream) {
    const chunks = []
    for await (const chunk of stream) chunks.push(chunk);
    return Buffer.concat(chunks).toString('utf8')
}

function advance(countArray) {
    let newState = new Array(POSSIBLE_STATES).fill(0)

    countArray.forEach((count, lifeStage) => {
        if (lifeStage === 0) {
            // regen
            newState[REBIRTH_AGE] += count
            newState[BABY_AGE] += count
        } else {
            newState[lifeStage - 1] += count
        }
    })

    return newState
}

(async () => {
    const input = await readAll(process.stdin)

    const fishes = input.trim().split(',').map(s => parseInt(s))

    let counts = new Array(POSSIBLE_STATES).fill(0)
    for (const fish of fishes)
        ++counts[fish];
    
    for (let i = 1; i <= 80; ++i) {
        counts = advance(counts)
        // const total = counts.reduce((acc, elem) => acc + elem)
        // console.log(`Day ${i}: ${total} fishes`)
    }
    const total = counts.reduce((acc, elem) => acc + elem)
    console.log(total)
})()

