// AlphaStrike by Team #1. AlphaNova courses🤙

using UnrealBuildTool;
using System.Collections.Generic;

public class AlphaStrikeEditorTarget : TargetRules
{
	public AlphaStrikeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange( new string[] { "AlphaStrike" } );
	}
}
