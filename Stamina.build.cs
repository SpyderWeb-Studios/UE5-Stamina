using UnrealBuildTool; 

public class Stamina: ModuleRules 

{ 

public Stamina(ReadOnlyTargetRules Target) : base(Target) 

{ 

PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
        PublicDependencyModuleNames.AddRange(new string[] { "Helper", "EnhancedInput", "UMG" });

    }

}