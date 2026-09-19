# Michelson-Morley model notes

FrameLab begins by comparing two mathematical models for an equal-arm Michelson interferometer.

## 1. Classical stationary-ether prediction

Assume light propagates at speed `c` in a preferred stationary ether frame and the entire apparatus moves through that frame at speed `v`.

For an arm of one-way length `L` whose direction makes angle `theta` with the velocity vector, solving the outgoing and return interception problems gives the exact Galilean round-trip time

`t(theta) = 2 L sqrt(c^2 - v^2 sin^2(theta)) / (c^2 - v^2)`.

Special cases:

- parallel: `t_parallel = 2 L c / (c^2 - v^2)`
- transverse: `t_perp = 2 L / sqrt(c^2 - v^2)`

The orthogonal arm is evaluated at `theta + 90 deg`.

The equivalent optical path difference is `c * (t_A - t_B)`, and dividing by wavelength gives the fringe offset. FrameLab also computes the change in that offset after rotating the entire instrument by 90 degrees.

For `L = 11 m`, `v ~= 29.78 km/s`, and `lambda = 500 nm`, the expected shift is on the order of 0.4 fringe.

## 2. Special relativity

In the interferometer rest frame, the vacuum speed of light is `c` in every direction. Equal arms therefore each have

`t = 2L/c`.

Their difference is zero and rotating the apparatus does not produce the classical ether-wind fringe shift.

## Important interpretation

This first comparison is intentionally between the original/simple stationary-ether prediction and special relativity. Later FrameLab modules can add the Lorentz-FitzGerald contraction model as a distinct historical model. That model was specifically constructed to reconcile an ether framework with null interferometer results, so it should not be conflated with the original uncontracted ether prediction.
