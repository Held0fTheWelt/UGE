# Unified Game Equipment

Developed with Unreal Engine 5

Current Version 5.6

## Features

## Main Branches 
- **master:** Stable version for production use.
- **EMPTY_BASE:** Empty base version for having clean branches to branch out from.
- **config:** Contains configuration files for the project.
- **build:** Contains the latest buildconfiguration of the project.
- **feature-all:** All features merged for testing and development.
- **feature-editor-all:** Branch for all editor-related features, including the manager and other editor tools.
- **dev:** Active development branch for new features and bug fixes.
- **staging:** Pre-release branch for final testing before merging into master.

## Core Plugins
Plugins that are essential for the core functionality of the equipment system and that are based on official learning and demo content.
- **feature-async-mixin:** C++ utility class for managing asynchronous operations like loading.
- **feature-common-loading-screen:** Loading screen manager handling creation and display of a project-specified loading screen widget.
- **feature-common-user:** Provides gameplay code and blueprint wrappers for online and platform operations.
- **feature-gameplay-message-router:** A subsystem that allows registering for and sending messages between unconnected gameplay objects.
- **feature-game-settings:** System for defining game-specific settings and exposing them to the UI.
- **feature-game-subtitles:** Game support for playing subtitles attached to media player assets.
- **feature-pocket-worlds:** Pocket Worlds

- ## Main Plugin Branches
- **feature-asset-loader:** Implements an asset loader for efficient management of game assets.
- **feature-input-system:** Develops a custom input system for handling player controls and interactions.

## Editor Branches
- **feature-editor-manager:** Main branch for editor development.
- **feature-editor-logging-macros:** Adds logging macros for better debugging and tracking in the editor.
