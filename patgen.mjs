import { readFileSync, writeFileSync } from 'node:fs';

function replacePattern(text) {
  return text.replace(/\{[^}]+\}/g, match => {
    const options = match.slice(1, -1).split('|');
    return options[Math.floor(Math.random() * options.length)];
  });
}

function generateTestCases(inputJson, patternText, count) {
  const input = JSON.parse(inputJson);
  
  for (let i = 0; i < count; i++) {
    const replacedText = replacePattern(patternText);
    const output = JSON.stringify(
      JSON.parse(inputJson.replace('"{{replace}}"', JSON.stringify(replacedText))),
      null,
      2
    );
    
    writeFileSync(`output_${i}.json`, output);
  }
}

const inputJson = readFileSync(process.argv[2], 'utf8');
const patternText = readFileSync(process.argv[3], 'utf8').trim();
const count = parseInt(process.argv[4]);

if (isNaN(count) || count <= 0) {
  console.error('Count must be a positive integer');
  process.exit(1);
}

generateTestCases(inputJson, patternText, count);