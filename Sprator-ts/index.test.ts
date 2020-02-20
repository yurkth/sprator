import test from 'ava';
import { promises as fs } from 'fs';
import path from 'path';
import { generate } from '.';

const dist = path.resolve(__dirname, 'dist');

test.before(async t => {
  await fs.mkdir(dist, { recursive: true });
});

test('generate 10x10', async t => {
  const seeds = [
    '00000000',
    '5bdd2c9c',
    'f1ebc493',
    'f32486ed',
    '040f1148',
    'b3907ac7',
    '76d10af1',
    '15063d76',
    '0e1557ad',
    '4a4c5f2f',
    'f072dddf',
    'ffffffff',
    'fffffffff' // same to ffffffff in 10x10
  ];

  for (const seed of seeds) {
    const small = generate(seed, 10, 1, '#228b22', '#2f4f4f', 2);
    t.snapshot(small);

    const large = generate(seed, 10, 10, '#228b22', '#2f4f4f', 2);
    await fs.writeFile(path.join(dist, seed + '.png'), large);
  }
});
