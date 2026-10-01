using System.IO;
using UnrealBuildTool;

public class FrameLabPhysics : ModuleRules
{
    public FrameLabPhysics(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;
        bEnableExceptions = true; // The portable core reports invalid input with exceptions.
        bUseUnity = false; // Compile the core translation units independently.
        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine" });

        string CoreRoot = Path.Combine(ModuleDirectory, "Private", "PhysicsCore");
        if (!Directory.Exists(CoreRoot))
            CoreRoot = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../../PhysicsCore"));
        if (!Directory.Exists(CoreRoot))
            throw new BuildException("FrameLab PhysicsCore is missing. Install with install_plugin.py.");
        PrivateIncludePaths.Add(Path.Combine(CoreRoot, "include"));
        PrivateIncludePaths.Add(CoreRoot);
    }
}
