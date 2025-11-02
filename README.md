Given:
n is the number of points on a circle.

Condition: n >= 5

x is the step size.

Condition: x < n / 2

Also: x does not divide n evenly (i.e., n % x ≠ 0)

Place n points on a circle.

From a starting point, draw a line to the point that is x steps away clockwise.

Repeat this from each newly reached point.

Continue until you return to the starting point.
