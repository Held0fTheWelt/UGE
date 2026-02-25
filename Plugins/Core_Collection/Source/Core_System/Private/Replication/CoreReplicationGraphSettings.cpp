// Copyright Epic Games, Inc. All Rights Reserved.
// Core_Collection

#include "Replication/CoreReplicationGraphSettings.h"
#include "Misc/App.h"
#include "Replication/CoreReplicationGraph.h"

UCoreReplicationGraphSettings::UCoreReplicationGraphSettings()
{
	CategoryName = TEXT("Game");
	DefaultReplicationGraphClass = UCoreReplicationGraph::StaticClass();
}
