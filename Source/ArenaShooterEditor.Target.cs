using UnrealBuildTool;

public class ArenaShooterEditorTarget : TargetRules
{
	public ArenaShooterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("ArenaShooter");
	}
}