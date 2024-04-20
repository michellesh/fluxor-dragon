const HEIGHT = 350;
const WIDTH = 274;

const XY = (x, y) => Object.freeze({ x, y });

const { PI } = Math;

const Ring = (radius, origin) => Object.freeze({
  cx: origin.x,
  cy: origin.y,
  origin,
  r: radius,
  draw: ctx => {
    ctx.beginPath();
    ctx.arc(origin.x, origin.y, radius, 0, 2 * Math.PI);
    ctx.stroke();
  }
});

const Arc = (radius, origin, startAngle, endAngle, counterclockwise = false) => Object.freeze({
  cx: origin.x,
  cy: origin.y,
  origin,
  r: radius,
  draw: ctx => {
    ctx.beginPath();
    ctx.arc(origin.x, origin.y, radius, startAngle, endAngle, counterclockwise);
    ctx.stroke();
  }
});

const Dot = (xy, color = 'black', radius = 2) => Object.freeze({
  x: xy.x,
  y: xy.y,
  draw: ctx => {
    ctx.beginPath();
    ctx.arc(xy.x, xy.y, radius, 0, 2 * Math.PI);
    ctx.fillStyle = color;
    ctx.fill();
  }
});

const Rectangle = (p1, p2, p3, p4) => Object.freeze({
  p1,
  p2,
  p3,
  p4,
  draw: ctx => {
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.lineTo(p3.x, p3.y);
    ctx.lineTo(p4.x, p4.y);
    ctx.fillStyle = 'rgba(0, 255, 255, 0.5)';
    ctx.fill();
  }
});

const Line = (p1, p2) => Object.freeze({
  p1,
  p2,
  draw: ctx => {
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.strokeStyle = 'black';
    ctx.stroke();
  },
  output: [
    '(',
    Math.round(p1.x),
    ',',
    Math.round(p1.y),
    ') -> (',
    Math.round(p2.x),
    ',',
    Math.round(p2.y),
    ')'
  ].join(' ')
});

const pointOnCircumference = (radius, origin, d) => {
  const degrees = d - 90 < 0 ? d + 270 : d - 90;
  const radians = (degrees * Math.PI) / 180;
  return XY(
    origin.x + radius * Math.cos(radians),
    origin.y + radius * Math.sin(radians)
  );
};

const pointOnLine = (p1, p2, howFar) => XY(
  p1.x + (p2.x - p1.x) * howFar,
  p1.y + (p2.y - p1.y) * howFar
);

/*
const p1 = XY(180, 80);
const p2 = XY(40, 240);
const arcCenters = [
  p1,
  pointOnLine(p1, p2, 0.2),
  pointOnLine(p1, p2, 0.4),
  pointOnLine(p1, p2, 0.6),
  pointOnLine(p1, p2, 0.8),
  p2
];
//const arcCenters = [
//  XY(180, 80),
//  XY(170, 100),
//  XY(160, 120),
//  XY(150, 140),
//  XY(140, 160),
//  XY(130, 180),
//];

const angleStep = 0.25;
const startAngle = PI;
const startAngles = d3.range(startAngle, startAngle - angleStep * 6 + .01, -angleStep);
//const startAngles = d3.range(PI * 0.98, PI * 0.49, (PI * 0.98 - PI * 0.5) / -5);
console.log('startAngles',startAngles);
//const endAngles = d3.range(PI * 0.52, PI * 0.29, (PI * 0.52 - PI * 0.3) / -5);
//console.log('endAngles',endAngles);

const leftWing = [
  Arc(OUTER_RADIUS, arcCenters[0], startAngles[0], startAngles[0] - PI * 0.4, true),
  Arc(OUTER_RADIUS, arcCenters[1], startAngles[1], startAngles[1] - PI * 0.4, true),
  Arc(OUTER_RADIUS, arcCenters[2], startAngles[2], startAngles[2] - PI * 0.4, true),
  Arc(OUTER_RADIUS, arcCenters[3], startAngles[3], startAngles[3] - PI * 0.4, true),
  Arc(OUTER_RADIUS, arcCenters[4], startAngles[4], startAngles[4] - PI * 0.4, true),
  Arc(OUTER_RADIUS, arcCenters[5], startAngles[5], startAngles[5] - PI * 0.4, true),
  //Arc(OUTER_RADIUS, arcCenters[5], PI * 1.0,  PI * 0.5,  PI * 1.0,  true),
];
*/

const wingLength = 84; // 84" = 7'
const wingTopY = 140;
const leftInnerX = 120;
const rightInnerX = leftInnerX + 108; // Belly about 108" across (9')
const wingStep = 14; // Wing rods are 14" apart
const leftInnerPts = [
  XY(leftInnerX, wingTopY),
  XY(leftInnerX, wingTopY + wingStep * 1),
  XY(leftInnerX, wingTopY + wingStep * 2),
  XY(leftInnerX, wingTopY + wingStep * 3),
  XY(leftInnerX, wingTopY + wingStep * 4),
  XY(leftInnerX, wingTopY + wingStep * 5),
];
const startAngle = -40;
const angleStep = -7;
const leftAngles = d3.range(startAngle, startAngle + (angleStep * 5) - 1, angleStep)
const rightAngles = leftAngles.map(angle => angle * -1);

const leftWingLines = [
  Line(leftInnerPts[0], pointOnCircumference(wingLength, leftInnerPts[0], leftAngles[0])),
  Line(leftInnerPts[1], pointOnCircumference(wingLength, leftInnerPts[1], leftAngles[1])),
  Line(leftInnerPts[2], pointOnCircumference(wingLength, leftInnerPts[2], leftAngles[2])),
  Line(leftInnerPts[3], pointOnCircumference(wingLength, leftInnerPts[3], leftAngles[3])),
  Line(leftInnerPts[4], pointOnCircumference(wingLength, leftInnerPts[4], leftAngles[4])),
  Line(leftInnerPts[5], pointOnCircumference(wingLength, leftInnerPts[5], leftAngles[5])),
];

const rightInnerPts = [
  XY(rightInnerX, wingTopY),
  XY(rightInnerX, wingTopY + wingStep * 1),
  XY(rightInnerX, wingTopY + wingStep * 2),
  XY(rightInnerX, wingTopY + wingStep * 3),
  XY(rightInnerX, wingTopY + wingStep * 4),
  XY(rightInnerX, wingTopY + wingStep * 5),
];
const rightWingLines = [
  Line(rightInnerPts[0], pointOnCircumference(wingLength, rightInnerPts[0], rightAngles[0])),
  Line(rightInnerPts[1], pointOnCircumference(wingLength, rightInnerPts[1], rightAngles[1])),
  Line(rightInnerPts[2], pointOnCircumference(wingLength, rightInnerPts[2], rightAngles[2])),
  Line(rightInnerPts[3], pointOnCircumference(wingLength, rightInnerPts[3], rightAngles[3])),
  Line(rightInnerPts[4], pointOnCircumference(wingLength, rightInnerPts[4], rightAngles[4])),
  Line(rightInnerPts[5], pointOnCircumference(wingLength, rightInnerPts[5], rightAngles[5])),
];

// Log pixel coordinates for each wing up to 8'.
// About 72 pixels in 8' of LED strip (8' * 12" / 1.3333")
const leftWingXY = d3.range(6).map(lineIndex =>
  d3.range(72).map(pixelIndex => {
    return pointOnCircumference(1.333333 * pixelIndex, leftInnerPts[lineIndex], leftAngles[lineIndex])
  })
);
const rightWingXY = d3.range(6).map(lineIndex =>
  d3.range(72).map(pixelIndex => {
    return pointOnCircumference(1.333333 * pixelIndex, rightInnerPts[lineIndex], rightAngles[lineIndex])
  })
);
console.log('leftWingXY',leftWingXY);
console.log('rightWingXY',rightWingXY);

// PRINT VALUES FOR C++
console.log('// left wing x values');
const leftWingX = leftWingXY.map(line => line.map(xy => Math.round(xy.x)))
leftWingX.forEach((line, lineIndex) => console.log(`uint16_t xLeft${lineIndex + 1}[] = {${line.join(', ')}};`));

console.log('// left wing y values');
const leftWingY = leftWingXY.map(line => line.map(xy => Math.round(xy.y)))
leftWingY.forEach((line, lineIndex) => console.log(`uint16_t yLeft${lineIndex + 1}[] = {${line.join(', ')}};`));

console.log('// right wing x values');
const rightWingX = rightWingXY.map(line => line.map(xy => Math.round(xy.x)))
rightWingX.forEach((line, lineIndex) => console.log(`uint16_t xRight${lineIndex + 1}[] = {${line.join(', ')}};`));

console.log('// right wing y values');
const rightWingY = rightWingXY.map(line => line.map(xy => Math.round(xy.y)))
rightWingY.forEach((line, lineIndex) => console.log(`uint16_t yRight${lineIndex + 1}[] = {${line.join(', ')}};`));

const draw = () => {
  const canvas = document.getElementById('canvas');
  if (canvas.getContext) {
    const ctx = canvas.getContext('2d');

    leftWingLines.forEach(line => {
      line.draw(ctx);
    });
    rightWingLines.forEach(line => {
      line.draw(ctx);
    });

    // draw a dot for pixel 15 line 2
    Dot(leftWingXY[2][15]).draw(ctx);
    Dot(rightWingXY[1][20]).draw(ctx);
  }
};
