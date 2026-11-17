
# **Forge**

| Author        | Ryan Leigh |
| ------------- | ---------- |
| Version       | 1.0        |
| Last Updated  | 11/17/2026 |

---

## Introduction

Forge is a custom 3D graphics engine developed in C++ using Microsoft's Direct3D 11 graphics API. Designed as a modular and extensible foundation for real-time graphics applications, Forge provides a collection of core engine systems responsible for graphics rendering, resource management, input processing, and application lifecycle management.

The engine abstracts low-level graphics functionality into reusable components, allowing developers to create and manage 3D scenes without directly interacting with the underlying graphics API. Its rendering capabilities include geometric primitives, mesh management, camera transformations, lighting, materials, and texture mapping. Additionally, Forge incorporates Dear ImGui to provide graphical user interface functionality for development tools and interactive application interfaces.

Forge emphasizes modularity, code reusability, and separation of concerns, allowing individual engine systems to operate independently while maintaining a cohesive architecture. This design enables Forge to serve as a reusable graphics framework for standalone applications and provides a foundation for future enhancements, including expanded scene management, rendering capabilities, and development tooling.

## Table of Contents

1. Objectives
2. System Overview
	1. Application
	2. Graphics
	3. Input
	4. UI
3. Classes
	1. Class Overview
	2. Class Diagram
4. Public Functions
5. Limitations and Future Considerations
	1. Current Limitations
	2. Future Improvements
6. Documentation
7. Example Project - Lunar Orbiter

## Objectives

The following are the primary objectives for Forge:

- **Modular Engine Architecture:** Establish a modular and extensible software architecture that separates core engine functionality into independent systems with clearly defined responsibilities.
  
- **Real-Time Rendering:** Provide real-time 3D rendering capabilities through Direct3D 11, supporting geometry rendering, camera transformations, lighting, materials, and texture mapping.
  
- **Application Integration:** Provide a reusable graphics framework that can be integrated into standalone applications without requiring direct interaction with low-level graphics APIs.
- **Input Handling:** Provide an input management system supporting keyboard and mouse interactions, allowing applications to respond to user input.
  
- **User Interface:** Integrate graphical user interface capabilities through Dear ImGui to support debugging, development tools, and interactive application interfaces.
  
- **Code Reusability:** Minimize duplicated functionality by providing reusable engine components and abstractions that can be shared across multiple applications.
  
- **Performance:** Maintain efficient CPU and GPU resource utilization to support responsive real-time rendering and interactive applications.

## _System Overview_

Forge is composed of several core systems that work together to provide a modular foundation for real-time 3D graphics applications. Each system is responsible for a distinct area of engine functionality while maintaining communication with other systems through the central `Application` class.

### _Application_

The `Application` class serves as the central coordinator of Forge. It manages the application lifecycle, initializes the engine's core systems, and coordinates their execution during runtime. It is responsible for maintaining the overall application state and ensuring that systems such as graphics, input, and user interface operate together.

### _Graphics_

The Graphics system provides the rendering functionality of Forge through Microsoft's Direct3D 11 API. Managed primarily by the `GraphicsManager` class, it handles graphics initialization, rendering resources, and frame presentation. It also coordinates mesh rendering, camera transformations, lighting, materials, and texture resources to produce the application's 3D scene.

### _Input_

The Input system handles keyboard and mouse interactions within the application. Managed by the `InputManager` class, it processes input events, tracks input states, and provides mechanisms for other engine components to respond to user interactions. This allows applications built with Forge to implement interactive controls and user-driven functionality.

### _UI_

The UI system provides graphical interface functionality through Dear ImGui. It allows applications to create interactive windows, controls, and development tools that integrate with the rendering pipeline. The system supports the creation of application-specific interfaces, such as scene hierarchies, property inspectors, debugging panels, and other interactive elements.



---

## **Classes**

Describe the important classes, interfaces, and their responsibilities.

### *Class Overview*

| Class           | Description                                                                                                                                                                                                                                                                       |
| --------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Application     | Serves as the primary entry point and central coordinator of a Forge application. Responsible for managing the application lifecycle, initializing core engine systems, coordinating runtime operations, and maintaining references to essential engine components and resources. |
| InputManager    | Handles keyboard and mouse input processing for the application. Responsible for tracking input states, processing input events, and providing an interface for other systems to register and respond to user interactions.                                                       |
| GraphicsManager | Manages the graphics subsystem and provides an abstraction over the Direct3D 11 API. Responsible for graphics initialization, device and context management, rendering configuration, resource management, and frame presentation.                                                |
| Window          | Encapsulates the application's native Windows window. Responsible for window creation, configuration, message processing, and providing the native window handle required by the graphics subsystem.                                                                              |
| Camera          | Represents the application's viewing perspective within a 3D scene. Responsible for maintaining camera transformations and is used to create the view and projection matrices that are used for the Model-View-Projection transformation.                                         |
| Mesh            | Represents geometric data used for rendering 3D objects. Responsible for managing vertex data, vertex and index buffers, the object's transformation.                                                                                                                             |
| Cube            | Subclass of Mesh which represents a predefined three-dimensional cube primitive.                                                                                                                                                                                                  |
| Icosphere       | Subclass of Mesh which epresents a spherical geometric primitive constructed from a subdivided icosahedron. Provides a relatively uniform distribution of vertices across its surface, making it suitable for spherical objects.                                                  |
| Skysphere       | Subclass of Icosphere that represents a spherical environment surrounding the scene. Essentially an inverted icosphere.                                                                                                                                                           |
| Material        | Defines the visual surface properties of a mesh. Responsible for managing material properties and associated textures, which determine how an object interacts with the rendering pipeline and lighting system.                                                                   |
| Texture         | Encapsulates texture resources used by the rendering system. Responsible for loading image data, managing texture resources, and providing the GPU texture and shader resource views required for texture sampling.                                                               |

### *Class Relationships*


![[Forge_2026-11-17_17-59-43.png]]

### **Public Functions**

All general public functions are defined in "framework.h"

| Method                 | Parameters                                  | Return Type           | Description                                                                                                                                                        |
| ---------------------- | ------------------------------------------- | --------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| ToWideString           | (const std::string& str)                    | std::wstring          | Helper function for converting a std::string into a std::wstring                                                                                                   |
| GetExecutableDirectory | ( )                                         | std::filesystem::path | Helper function for determining the directory of the application's executable file                                                                                 |
| GetResourcePath        | (const std::filesystem::path& relativePath) | std::filesystem::path | Helper function for determining the absolute path from a relative path. Useful so that the application can determine paths from any directory such as the desktop. |


---

## **Engine Workflow**

This section describes the operational workflow of Forge, including application initialization, runtime execution, rendering, and shutdown. Forge follows a centralized application lifecycle in which the `Application` class coordinates the initialization and interaction of the engine's core systems.

### *Initialization*

During initialization, Forge establishes the resources and systems required to execute a real-time graphics application. The `Application` class acts as the central coordinator, initializing the window, graphics subsystem, input manager, and other required engine components.

The initialization process generally follows these steps:

1. **Application Creation:** An instance of the `Application` class is created, establishing the primary application context.
2. **Window Initialization:** The `Window` class creates and configures the native Windows application window.
3. **Graphics Initialization:** The `GraphicsManager` initializes the Direct3D 11 device, device context, swap chain, and associated rendering resources.
4. **Input Initialization:** The `InputManager` is initialized to receive and process keyboard and mouse input.
5. **Resource Initialization:** Required meshes, textures, materials, shaders, and other application resources are loaded or created.
6. **Camera Initialization:** The camera is configured with its initial position, orientation, and projection settings.
7. **GUI Initialization:** Dear ImGui is initialized and configured with the appropriate platform and Direct3D 11 backends.

Upon successful initialization, Forge transitions into its main application loop.

### *Runtime*

Once initialization is complete, Forge enters its primary execution loop. This loop is responsible for processing application events, updating engine systems, rendering the scene, and presenting the completed frame.

Each iteration of the application loop generally consists of the following operations:

1. **Event Processing:** The application processes Windows messages and handles events such as window resizing, closing, and user input.
2. **Input Processing:** The `InputManager` processes keyboard and mouse events, updating the current input state.
3. **Frame Initialization:** The `GraphicsManager` prepares the rendering frame by clearing the render target and depth buffer.
4. **Scene Rendering:** Visible meshes are submitted for rendering. Their associated geometry, materials, textures, and transformation data are used to configure the rendering pipeline and issue Direct3D 11 draw calls.
5. **GUI Rendering:** Dear ImGui generates and renders the graphical user interface, allowing applications to display interactive controls, debugging information, and development tools.
6. **Frame Presentation:** The `GraphicsManager` presents the completed frame through the Direct3D 11 swap chain.

These operations repeat until the application receives a termination request.

###  *Shutdown*

When the application loop terminates, Forge performs its shutdown procedure to release resources and properly terminate the application.

The shutdown process generally includes:

1. **GUI Shutdown:** Dear ImGui and its associated platform and rendering backends are shut down.
2. **Resource Cleanup:** Application-owned meshes, textures, materials, shaders, and other allocated resources are released.
3. **Graphics Cleanup:** Direct3D 11 resources, including the swap chain, render targets, device context, and device, are released when no longer needed.
4. **Input Cleanup:** The `InputManager` releases any resources associated with input processing.
5. **Window Destruction:** The native application window is destroyed and its associated resources are released.
6. **Application Termination:** The `Application` instance completes its shutdown procedure and the program exits.

### *Rendering Sequence Diagram*

The rendering sequence describes how Forge coordinates its engine components to produce an individual frame.

The `Application` initiates the rendering process by requesting that the `GraphicsManager` begin a new frame. The `GraphicsManager` prepares the rendering targets, after which the application iterates through each mesh and requests that it be drawn.

During mesh rendering, the mesh provides its geometric data and associated material information. Textures and other material resources are made available to the graphics pipeline, allowing the appropriate shaders and rendering resources to be bound before issuing draw calls.

After the scene has been rendered, Forge processes the Dear ImGui interface and submits its draw data to the Direct3D 11 backend. Finally, the `GraphicsManager` presents the completed frame to the application window.

![[forge_sequence_diagram.png]]

---

## **External Dependencies**

The following APIs are required by Forge, however only the Windows SDK needs to be installed. It can be installed by checking the **Desktop development with C++** (or UWP) workload during Visual Studio setup or from the Microsoft Windows SDK Download Page:  https://learn.microsoft.com/en-us/windows/apps/windows-sdk/

| Dependency    | Type                           | Integration                        |
| ------------- | ------------------------------ | ---------------------------------- |
| Windows SDK   | Windows Development SDK        | Windows SDK headers and libraries  |
| Direct3D 11   | Windows Grapics API            | Provided by Windows SDK            |
| DirectXMath   | SIMD linear algebra library    | Provided by Windows SDK            |
| Dear ImGui    | Graphical User Interface API   | Provided by Forge.dll              |
| stb_image     | image loading and decoding API | Provided by Forge.dll              |
| tinyobjloader | .obj file loader               | Provided by Forge.dll              |


---

## **Limitations and Future Considerations**

### *Current Limitations*

- Only one Texture per Material
- Only one Material per Mesh
- Only shading model supports are Flat Shading and Unlit Shading
- Lack of physics engine and collision detection
- Lack of support for animations

### *Future Considerations*

#### Actor and Component System

Introduce an `Actor` class to represent higher-level scene entities capable of containing multiple meshes and subcomponents. This would allow individual objects to be composed of multiple interconnected elements rather than being limited to a single mesh.

Potential functionality includes:

- Support for multiple meshes within a single actor.
- Hierarchical component relationships.
- Parent-child transformations.
- Independent component properties and behaviors.

#### Multiple Textures per Material

Expand the material and texture systems to support multiple textures per material. This would provide greater flexibility when defining surface appearance and enable more advanced rendering techniques.

#### Rendering

Expand upon the current capabilities of the renderer. 

Potential functionality include:

- Blinn-Phong specular shading
- Cell Shading
- Gooch Shading
- Ambient Occlusion
- Global Illumination
- Physically Based Rendering (PBR)
- Spot lights
- Directional lights
- Area lights
- Bloom
- Gamma correction
- Color grading
- Depth of field
- Motion blur
- Screen-space Reflections

#### Scene Manager

Introduce a centralized `SceneManager` responsible for managing the lifecycle and organization of scene objects within a Forge application.

Potential functionality includes:

- Creating, loading, and unloading scenes.
- Managing actors and scene objects.
- Maintaining scene hierarchies.
- Adding and removing objects dynamically.
- Managing active scenes and scene transitions.
- Integrating with the proposed Actor and Component System.

### Animations

Introduce an animation subsystem to Forge to support animated objects, characters, and environmental elements within 3D scenes. The system would provide mechanisms for defining, controlling, and blending animations, enabling more dynamic and interactive applications.

##### *Skeletal Animation*

Implement a skeletal animation system to support character and object animations through hierarchical bone structures.

Potential functionality includes:

- Skeleton and bone hierarchy management.
- Bone transformations and hierarchical animation.
- Skinning and vertex weight support.
- GPU-based skeletal animation.
- Animation clips and playback controls.
- Support for animated 3D model formats.

##### *Animation Playback and Control*

Introduce an animation controller responsible for managing animation playback and execution.

Potential functionality includes:

- Animation playback, pausing, and stopping.
- Playback speed and time control.
- Looping and one-shot animations.
- Animation clip selection.
- Animation state management.
- Animation events and callbacks.

##### *Animation Blending*

Implement animation blending to support smooth transitions between different animation states and movements.

Potential functionality includes:

- Linear interpolation between animation poses.
- Crossfading between animation clips.
- Blending multiple animation layers.
- Weighted animation blending.
- Transition duration and blending controls.

##### *Animation State Machines*

Introduce animation state machines to manage complex animation behaviors and transitions based on application or actor state.

Potential functionality includes:

- Animation states and transitions.
- Conditional transition rules.
- Parameter-driven animation selection.
- State entry and exit events.
- Integration with actor behavior and input systems.

##### *Transform Animation*

Expand animation capabilities beyond skeletal animation to support transformations of individual scene objects and components.

Potential functionality includes:

- Position animation.
- Rotation animation.
- Scale animation.
- Keyframe-based animation.
- Interpolation between keyframes.
- Animation curves and easing functions.

##### *Animation Import and Asset Support*

Implement mechanisms for importing and managing externally authored animation assets.

Potential functionality includes:

- Skeletal animation data import.
- Animation clip extraction.
- Skeleton hierarchy import.
- Vertex weight and bone index processing.
- Support for animated FBX models.
- Animation asset management.

##### *Inverse Kinematics (IK)*

Introduce inverse kinematics to calculate bone transformations based on desired positions or constraints.

Potential functionality includes:

- Two-bone inverse kinematics.
- End-effector positioning.
- Joint constraints.
- Procedural limb positioning.
- Foot placement and ground alignment.
  
### Physics Engine

Introduce a physics subsystem to Forge to support physical simulations, collision detection, and dynamic interactions between objects within a 3D scene. The system would provide a foundation for creating interactive environments with realistic object behavior and physical responses.

###### *Rigid Body Dynamics*

Implement rigid body physics to simulate the movement and interaction of solid objects.

Potential functionality includes:

- Static, dynamic, and kinematic rigid bodies.
- Mass, density, and inertia properties.
- Linear and angular velocity.
- Force and torque application.
- Gravity and damping.
- Collision response and impulse resolution.

##### *Collision Detection*

Introduce collision detection to identify intersections and physical interactions between objects.

Potential functionality includes:

- Bounding box collision detection (AABB).
- Oriented bounding boxes (OBB).
- Bounding sphere collision detection.
- Bounding capsule collision detection.
- Mesh-based collision detection.
- Broad-phase and narrow-phase collision detection.
- Collision filtering and collision layers.

##### *Constraints and Joints*

Introduce a constraint system to define physical relationships between rigid bodies.

Potential functionality includes:

- Fixed joints.
- Hinge joints.
- Slider joints.
- Spring constraints.
- Distance constraints.
- Configurable joint limits.

##### *Raycasting and Spatial Queries*

Provide mechanisms for querying the scene environment to identify objects and locations within a scene.

Potential functionality includes:

- Raycasting
- Shape casting
- Overlap queries
- Collision filtering
- Surface normal and intersection point retrieval

---

## **Documentation**

- Forge:  TODO: Link to Lunar Orbiter
- Dear ImGui: https://github.com/ocornut/imgui/wiki

## **Example Project - Lunar Orbiter**

**Lunar Orbiter** is an example application built using the Forge graphics engine that demonstrates its 3D rendering capabilities. The application features a textured 3D representation of the Moon, allowing users to explore its surface, interact with geographic feature markers, and view detailed information about notable lunar landmarks. It serves as a practical demonstration of Forge's mesh rendering, texture mapping, camera controls, lighting, and Dear ImGui integration.

TODO: Link to Lunar Orbiter