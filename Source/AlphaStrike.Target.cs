// AlphaStrike by Team #1. AlphaNova courses🤙

using UnrealBuildTool;
using System.Collections.Generic;

public class AlphaStrikeTarget : TargetRules
{
	public AlphaStrikeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange( new string[] { "AlphaStrike" } );
	}
}
