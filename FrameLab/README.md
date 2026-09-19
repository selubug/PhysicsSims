# FrameLab

**FrameLab** is a modern C++ physics-simulation core designed to power interactive Unreal Engine visualizations of important experiments and competing physical models.

The first module implements the **Michelson-Morley interferometer** and compares:

- the original/simple stationary-ether prediction;
- special relativity in the interferometer rest frame.

The scientific code is deliberately independent of Unreal Engine. Unreal will act as the real-time 3D visualization and interaction layer, while this library remains testable from any standard C++20 toolchain.

## Current milestone

- reusable `IMichelsonMorleyModel` interface
- exact arbitrary-angle stationary-ether travel-time calculation
- special-relativity model
- 90-degree rotation fringe-shift calculation
- 0-180 degree simulation sweep
- CSV export for visualization
- automated tests
- Unreal integration blueprint / adapter plan

## Architecture

```text
                     Unreal Engine (next layer)
               UI / Actors / beams / materials
                             |
                     thin Unreal adapter
                             |
                +------------+------------+
                |       PhysicsCore       |
                |      pure C++20         |
                +------------+------------+
                             |
          +------------------+------------------+
          |                                     |
 ClassicalEtherModel                 SpecialRelativityModel
          |                                     |
          +------------------+------------------+
                             |
                 MichelsonMorleyResult
```

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Run the demo:

```bash
./build/framelab_cli
```

On a multi-config generator such as Visual Studio, the executable may be under `build/Debug/` or `build/Release/`.

The program also writes `michelson_morley_sweep.csv` containing a 1-degree sweep from 0 through 180 degrees.

## Example parameters

The default demo uses:

- one-way modeled arm length: `11 m`
- wavelength: `500 nm`
- hypothetical ether speed: `29.78 km/s`
- starting orientation: `0 degrees`

These values produce the historically relevant order of magnitude: roughly four-tenths of a fringe predicted by the simple stationary-ether model after a 90-degree rotation, while the special-relativity model returns zero orientation-dependent shift.

See [`docs/PHYSICS.md`](docs/PHYSICS.md) for the derivation and interpretation.


## Planned modules

- Lorentz-FitzGerald contraction model
- light-clock / Lorentz-transformation visualizer
- Newtonian orbital mechanics
- numerical integrators (Euler, Verlet, RK4)
- N-body gravity / three-body chaos
- relativistic corrections and comparison experiments
