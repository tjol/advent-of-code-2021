#!/usr/bin/env node

'use strict';

async function readAll(stream) {
    const chunks = []
    for await (const chunk of stream) chunks.push(chunk);
    return Buffer.concat(chunks).toString('utf8')
}

function polymerizeOnce(polymer, rulebook) {
    let result = [polymer[0]]

    for (let i = 1; i < polymer.length; ++i) {
        const pair = polymer[i-1] + polymer[i]
        const insertion = rulebook[pair]
        if (insertion != undefined) {
            result.push(insertion)
        }
        result.push(polymer[i])
    }

    return result
}

function computeHistogram(polymer) {
    let hist = {}
    for (const c of polymer) {
        const n = hist[c]
        if (n == undefined)
            hist[c] = 1
        else
            hist[c] = n + 1
    }
    return hist
}

(async () => {
    const input = await readAll(process.stdin)

    const lines = input.split('\n')
    const template = Array.from(lines[0].trim())

    let mapping = {}
    for (const line of lines.slice(2)) {
        const [from, to] = line.trim().split(' -> ')
        mapping[from] = to
    }

    let polymer = template
    for (let i = 0; i < 10; ++i)
        polymer = polymerizeOnce(polymer, mapping)

    const histogram = computeHistogram(polymer)

    let minLetter = null
    let minCount = polymer.length
    let maxLetter = null
    let maxCount = 0
    for (const c in histogram) {
        let count = histogram[c]
        if (count < minCount) {
            minCount = count
            minLetter = c
        }
        if (count > maxCount) {
            maxCount = count
            maxLetter = c
        }
    }

    console.log(maxCount - minCount)
})()
