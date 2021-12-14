#!/usr/bin/env node

'use strict';

async function readAll(stream) {
    const chunks = []
    for await (const chunk of stream) chunks.push(chunk);
    return Buffer.concat(chunks).toString('utf8')
}

function addToHist(hist, key, delta = 1) {
    hist[key] = (hist[key] ?? 0) + delta
}

function polymerizeOnce(polymerPairs, rulebook) {
    let result = {}

    for (const [pair, count] of Object.entries(polymerPairs)) {
        const insertion = rulebook[pair]
        if (insertion == undefined) {
            addToHist(result, pair, count)
        } else {
            addToHist(result, insertion[0], count)
            addToHist(result, insertion[1], count)
        }
    }

    return result
}

function extractPairs(polymer) {
    let pairHistogram = {}
    for (let i = 0; i < polymer.length - 1; ++i) {
        const pair = polymer[i] + polymer[i+1]
        addToHist(pairHistogram, pair)
    }
    return pairHistogram
}

function computeLetterFrequencies(polymerPairs, startLetter, endLetter) {
    let hist = {[startLetter]: 0.5, [endLetter]: 0.5}

    for (const [pair, count] of Object.entries(polymerPairs)) {
        addToHist(hist, pair[0], count / 2)
        addToHist(hist, pair[1], count / 2)
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
        mapping[from] = [from[0] + to, to + from[1]]
    }

    let polymerPairs = extractPairs(template)

    for (let i = 0; i < 10; ++i) {
        polymerPairs = polymerizeOnce(polymerPairs, mapping)
    }

    const histogram = computeLetterFrequencies(polymerPairs, template[0], template[template.length-1])

    let minLetter = null
    let minCount = Infinity
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
