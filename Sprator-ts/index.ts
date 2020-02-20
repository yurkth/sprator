import { Canvas } from 'canvas';

type Bin = 0 | 1;

/**
 * Generate 8bit sprite
 * @param seed Hex string represents seed of generation. e.g. ffffffff
 * @param dotSize Number of dots in an edge. 6 or more (must be even)
 * @param pixelsPerDot Numbrer of pixels in a dot. e.g. If dotSize=10 and pixelsPerDot=4, image size is 40x40.
 * @param fillColor Fill color can be used on the Canvas. e.g. #000000
 * @param borderColor Border color can be used on the Canvas. e.g. #000000
 * @param stepCount Number of times to apply the algorithm
 */
export function generate(
  seed: string,
  dotSize: number,
  pixelsPerDot: number,
  fillColor: string,
  borderColor: string,
  stepCount = 2
) {
  // Validate
  if (dotSize < 6) {
    throw new TypeError(`dotSize must be 6 or more but ${dotSize}.`);
  }
  if (dotSize % 2 !== 0) {
    throw new TypeError(`dotSize must be even number but ${dotSize}.`);
  }

  let matrix = initialize(seed, dotSize);
  for (let index = 0; index < stepCount; index++) {
    matrix = step(matrix);
  }
  return render(matrix, pixelsPerDot, fillColor, borderColor);
}

function initialize(seed: string, spriteSize: number) {
  const getBinaly = (position: number) => {
    const hex = seed[(position / 4) >> 0] || '0'; // padding with 0
    const bin = (parseInt(hex, 16) >>> (3 - (position % 4))) & 1; // bit at position
    return bin as Bin;
  };
  const matrix: Bin[][] = [];
  for (let y = 0; y < spriteSize; y++) {
    const row = Array.from<Bin>({ length: spriteSize / 2 }).fill(0);
    matrix[y] = row; // Fill 0
    if (y === 0 || y === spriteSize - 1) continue; // Fill top/bottom edge with 0
    row[0] = 0; // Fill left edge with 0
    for (let x = 1; x < row.length; x++) {
      row[x] = getBinaly(x - 1 + (y - 1) * (spriteSize / 2 - 1));
    }
  }
  return matrix;
}

function step(prev: Bin[][]) {
  const next: Bin[][] = [];
  const size = prev.length;
  for (let y = 0; y < size; y++) {
    const row = (next[y] = Array.from<Bin>({ length: size / 2 }).fill(0)); // Fill 0
    if (y === 0 || y === size - 1) continue; // Fill top/bottom edge with 0
    row[0] = 0; // Fill left edge with 0
    for (let x = 1; x < row.length; x++) {
      // cellular automaton
      const n = countNeighbors(prev, x, y);
      row[x] = prev[y][x] === 0 ? (n <= 1 ? 1 : 0) : n === 2 || n === 3 ? 1 : 0;
    }
  }
  return next;
}

function render(
  matrix: Bin[][],
  ppd: number,
  fillColor: string,
  borderColor: string
) {
  const size = matrix.length;
  const canvas = new Canvas(size * ppd, size * ppd);
  const ctx = canvas.getContext('2d');
  for (let y = 0; y < matrix.length; y++) {
    const row = matrix[y];
    for (let x = 0; x < row.length; x++) {
      if (row[x] === 1) {
        ctx.fillStyle = fillColor;
      } else if (countNeighbors(matrix, x, y) > 0) {
        ctx.fillStyle = borderColor;
      } else {
        continue;
      }

      ctx.fillRect(x * ppd, y * ppd, ppd, ppd); // left side
      ctx.fillRect((size - x - 1) * ppd, y * ppd, ppd, ppd); // right side (mirror)
    }
  }
  return canvas.toBuffer();
}

function countNeighbors(matrix: Bin[][], x: number, y: number) {
  return (
    (matrix[y - 1]?.[x] || 0) +
    (matrix[y + 1]?.[x] || 0) +
    (matrix[y][x - 1] || 0) +
    (matrix[y][x + 1] || 0)
  );
}
