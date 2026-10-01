# FrameLab Unreal integration

This Unreal Engine 5 runtime plugin connects the portable C++20 PhysicsCore to Blueprints. It includes a component for simulation calls and an actor that draws a rotating interferometer with both model predictions. No external service, network connection, or CSV import is needed: Unreal compiles and calls the same core sources as the CLI.

## Install into your Unreal project

Use a UE5 installation with its supported C++ compiler/toolchain. Close the editor before installing.

From this repository's root, run:

```bash
python FrameLab/UnrealIntegration/install_plugin.py "/path/to/YourProject/YourProject.uproject"
```

The installer creates `YourProject/Plugins/FrameLabPhysics`, including a snapshot of the current PhysicsCore. It does not modify your `.uproject` or overwrite an existing plugin. To update an installation, back up and remove its `Plugins/FrameLabPhysics` directory, then reinstall. Reinstall after changing the core in this repository.

Alternatively, extract the packaged `FrameLabPhysics.zip` into your project's `Plugins` directory. The resulting path must be `Plugins/FrameLabPhysics/FrameLabPhysics.uplugin`. Copying the source plugin folder alone is insufficient outside this repository because it needs the shared core; use the installer or packaged zip.

1. Generate project files and build your project's **Development Editor** target. A Blueprint-only project may need a C++ class added through Unreal first to create the game target and compiler setup.
2. Open the editor, enable **FrameLab Physics** under **Edit > Plugins**, and restart if prompted.
3. Create a Blueprint subclass of **FrameLabInterferometerActor**, place it near your player/camera in a level, and enable **Auto Rotate** in its Details panel.
4. Press Play. Cyan and yellow lines show the perpendicular arms; the green arrow is the fixed ether-wind reference. World-space text compares offsets, 90-degree rotation shifts, and ether arm travel times.

The debug drawing is a development preview, visible during Play and normally excluded from Shipping builds. There are no prebuilt maps, meshes, or UMG widgets. For a shipping presentation, add meshes/materials and a widget in the Blueprint subclass, using the API below. Move the camera so the apparatus and labels are visible.

## Blueprint API

Add a **Michelson Morley Component** to any actor, or use the preview actor's **Simulation** component.

- **Simulate** selects `ClassicalEther` or `SpecialRelativity` and returns one result.
- **Compare Models** evaluates both models with the same inputs and returns two results.
- Both calls have execution pins and return a boolean success value and an error string. Always branch on success. Failed calls clear their outputs so a UI cannot accidentally reuse a previous result.

Inputs:

| Input | Unit | Default |
| --- | --- | --- |
| Arm Length Meters | m, one-way length | 11 |
| Wavelength Nanometers | nm | 500 |
| Frame Speed Meters Per Second | m/s, hypothetical ether-relative speed | 29780 |
| Orientation Degrees | degrees relative to the local ether-wind direction | 0 |

Inputs must be finite. Length and wavelength must be positive; speed must satisfy `0 <= v < 299792458`. The component also rejects non-finite numerical results. Orientation is normalized before evaluation. This remains a double-precision educational model: extreme inputs can lose accuracy even when finite.

Outputs are `ModelName`, `ArmATimeSeconds`, `ArmBTimeSeconds`, `TimeDifferenceSeconds`, `OpticalPathDifferenceMeters`, `FringeOffset`, and `RotationFringeShift`. The final value is the **signed change** in fringe offset after an additional 90-degree apparatus rotation, not its absolute magnitude. SR is evaluated in the apparatus rest frame.

For custom presentation:

1. Use sliders to set the preview actor's physics properties.
2. Call **Refresh Simulation** after changing inputs, or use the actor's per-frame refresh while playing.
3. Implement **On Simulation Updated** in the Blueprint subclass. Branch on its success parameter and update your UMG widget from **Ether Result** / **Relativity Result**, or show **Last Error**.
4. Use **Orientation Degrees** to rotate your visual arm assembly relative to its parent. Auto Rotate changes that value; it does not rotate added meshes automatically.

**Display Scale** and the actor transform only affect preview geometry. Meters convert to Unreal centimeters as `length * 100 * DisplayScale`. They never change the physical arm length, speed of light, travel times, or fringe predictions. Any added slow-motion light markers or exaggerated fringe materials must be explicitly labeled as presentation effects. The preview shows arm geometry and numerical predictions; it does not animate physical photon trajectories or reproduce interference optics.

## Source layout

- `FrameLabPhysics/Source/FrameLabPhysics/Public/MichelsonMorleyComponent.h`: Blueprint enum, result structure, and API.
- `Private/MichelsonMorleyComponent.cpp`: SI conversion, core calls, error handling, result mapping.
- `Public/FrameLabInterferometerActor.h` and `Private/FrameLabInterferometerActor.cpp`: development preview and Blueprint presentation hook.
- `Private/*Core.cpp`: compile the original PhysicsCore translation units.
- `FrameLabPhysics.Build.cs`: C++20, core include paths, exception support, module dependencies.
- `install_plugin.py`: package core and plugin together. Packaged core `.cpp` files become `.inl` includes so UnrealBuildTool does not compile them twice. Their source content is unchanged.

## Verification

Portable tests cover the historical prediction, the SR null result, nanometer conversion, equivalent orientations, invalid display inputs, packaging, and refusal to overwrite an existing installation:

```bash
cmake -S FrameLab -B FrameLab/build
cmake --build FrameLab/build
ctest --test-dir FrameLab/build --output-on-failure
```

Engine automation test: open **Tools > Test Automation** / **Session Frontend > Automation** (menu location varies by UE version), and run `FrameLab.Bridge.ModelComparison`. It checks the actual component against the core and exercises error handling.

Final verification: the CMake Release build succeeded and all three CTest suites passed (core predictions, display-unit input validation, and plugin packaging). The packaged core was also compiled independently and tested from an extracted plugin archive. Unreal Engine was not available in that environment, so UnrealHeaderTool, UnrealBuildTool, the engine automation test, and the in-editor preview still require verification in your UE5 project. This plugin uses UE5 double-valued Blueprint pins and `TObjectPtr`; it does not target UE4.
