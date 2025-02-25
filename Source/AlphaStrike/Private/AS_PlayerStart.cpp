﻿// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AS_PlayerStart.h"

void AAS_PlayerStart:: SetIsOccupied(bool IsOccupied)
{
    if (IsOccupied)
    {
        GetWorldTimerManager().SetTimer(OccupiedTimer, 
            FTimerDelegate::CreateLambda([&] { bIsOccupied = false; }), OccupiedTime, false);
    }
    bIsOccupied = IsOccupied;
}
