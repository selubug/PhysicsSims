FrameLab is a C++20 physics simulation library focused on comparing competing physical models in a way that is both mathematically explicit and easy to visualize.

The current implementation centers on a Michelson-Morley interferometer and compares:

- a classical stationary-ether model
- a special-relativity model

This project is intentionally separated from Unreal Engine: the physics logic lives in a pure C++ core that can be tested, benchmarked, and reused independently, while an Unreal layer can later provide immersive real-time visualization.

## Why this project exists

FrameLab is designed to make the historical physics comparison concrete:

- model the same experimental setup under different assumptions
- compute fringe shifts and optical path differences
- export sweep data for plotting or animation
- keep the physics engine reusable across tools and frontends

## Current capabilities

- exact travel-time calculation for a Michelson-Morley interferometer at arbitrary arm angle
- comparison of stationary-ether and special-relativity predictions
- 90-degree rotation fringe-shift analysis
- 0-180 degree sweep generation
- CSV export for downstream visualization
- automated CTest coverage for the core experiment logic

## Repository layout

```text
FrameLab/
├── PhysicsCore/        # physics models and calculation code
├── apps/               # CLI demo application
├── docs/               # derivations and notes
├── tests/              # automated tests
├── UnrealIntegration/  # planned Unreal bridge / adapter layer
├── CMakeLists.txt      # build configuration
├── README.md           # project overview
└── .gitignore
```

## Build and run

From the repository root:

```bash
cmake -S FrameLab -B FrameLab/build
cmake --build FrameLab/build
ctest --test-dir FrameLab/build --output-on-failure
```

Run the demo:

```bash
./FrameLab/build/framelab_cli
```

The demo generates `michelson_morley_sweep.csv`, containing a 1-degree sweep from 0 through 180 degrees.

## Example physical setup

The default demo models:

- arm length: 11 m
- wavelength: 500 nm
- ether speed: 29.78 km/s
- starting orientation: 0 degrees

This produces the historically relevant order-of-magnitude comparison between the classical ether prediction and the special-relativity result.

## Documentation

- `docs/PHYSICS.md` describes the underlying derivation and interpretation

## Planned modules

FrameLab is intended to grow beyond the Michelson-Morley comparison into a broader physics simulation toolkit, including:

- Lorentz-FitzGerald contraction models
- light-clock and Lorentz-transformation visualizations
- Newtonian orbital mechanics
- numerical integrators (Euler, Verlet, RK4)
- N-body gravity and chaotic-system examples
- additional relativity and comparison experiments

## License

This project does not currently declare a repository license.

## Status

FrameLab is a working prototype for physics-model comparison and visualization, with a strong emphasis on testable numerical behavior and future integration with real-time 3D tooling.
