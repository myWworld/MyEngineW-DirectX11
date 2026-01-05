# MyEngineW-DirectX11

> DirectX11 기반으로 Rendering Pipeline, Component System,  
> Resource Management를 직접 구현한 개인 3D Game Engine 프로젝트입니다.

Windows API 기반 2D 엔진 구조를 출발점으로 삼아,  
3D 렌더링 파이프라인과 컴포넌트 기반 아키텍처를 갖춘 엔진으로 확장하는 것을 목표로 개발했습니다.

본 프로젝트는 **Lighting 모델 구현 이전 단계의 엔진 골격 구축**에 집중하며,  
렌더링 흐름, 리소스 관리, 게임 오브젝트 구조를 명확히 설계하는 데 중점을 두었습니다.

---

## 🚀 Project Overview

- **Language / Tech**
  - C++ (Modern C++)
  - Windows API (Win32)
  - DirectX 11
  - HLSL (Shader Model 5)

- **Development Environment**
  - Windows 10+
  - Visual Studio

- **Project Goal**
  - DirectX11 기반 Rendering Pipeline 이해 및 구현
  - Component-based Architecture 설계
  - Mesh Rendering, Camera, Basic Physics 구조 구현
  - Lighting 모델을 제외한 엔진의 핵심 뼈대 구축

- **Scope**
  - Mesh Rendering
  - Transform / Collider / Rigidbody Component
  - Camera System
  - Scene & Object Lifecycle Management

---

## 🏗️ Architecture Overview

본 엔진은 **Core / Graphics / Components / Resources** 계층으로 분리된 구조를 가지며,  
각 계층은 명확한 책임을 갖도록 설계되었습니다.



## 🏗️ 아키텍처 및 디렉토리 구조
/MyEngineW-DirectX11
│
├─ Core/
│    Engine.cpp / Engine.h           ← 엔진 초기화, 루프, 업데이트 호출  
│    WindowManager.cpp / .h          ← Win32 창 생성, 메시지 루프 처리  
│    TimeManager.cpp / .h            ← DeltaTime 계산, 프레임 타이밍  
│    InputManager.cpp / .h           ← 키보드/마우스 이벤트 처리  
│
├─ Graphics/
│    Device.cpp / .h                 ← Direct3D11 Device/Context/SwapChain 초기화  
│    Renderer.cpp / .h               ← DrawCall 관리, 렌더링 흐름 제어  
│    Shader.cpp / .h                 ← HLSL Shader 로딩 및 바인딩  
│    Mesh.cpp / .h                   ← 메시 자료구조 및 로딩 인터페이스  
│    Camera.cpp / .h                 ← View, Projection 행렬 계산  
│
├─ Components/
│    Transform.cpp / .h              ← 위치(Position), 회전(Rotation), 스케일(Scale) 관리  
│    MeshRenderer.cpp / .h           ← Mesh + Material 렌더러  
│    Rigidbody.cpp / .h              ← 속도, 중력 등 물리 계산  
│    Collider.cpp / .h               ← 충돌 감지 (AABB, Sphere 등)  
│
├─ Math/
│    Vector3.cpp / .h                ← 3D 벡터 연산  
│    Matrix4x4.cpp / .h              ← 4×4 행렬 변환 (World, View, Projection)  
│    Quaternion.cpp / .h             ← 회전 변환 용 쿼터니언  
│
├─ Resources/
│    MeshLoader.cpp / .h             ← OBJ 또는 FBX 기반 메시 로딩 (Assimp 등 사용 가능)  
│    Material.cpp / .h               ← 셰이더 + 텍스처 + 세팅을 포함하는 머티리얼 정보  
│
└─ Entry/
     Main.cpp                         ← WinMain 또는 main 진입점, 엔진 실행 흐름  


---

## 🎨 Rendering Pipeline

### Frame Flow

1. **CPU Side**
   - Scene으로부터 Renderable Object 수집
   - Camera 기준 View / Projection Matrix 계산
   - Constant Buffer 업데이트

2. **GPU Side**
   - Vertex Shader
     - World / View / Projection 변환
   - Pixel Shader
     - 기본 색상 출력 (Lighting 미적용)

3. **Rendering Loop**
   - Clear RenderTargetView / DepthStencilView
   - DrawIndexed 호출
   - SwapChain Present

본 구조는 Lighting, Shadow, Post-Processing 등  
추가 렌더링 패스를 자연스럽게 확장할 수 있도록 설계되었습니다.

---

## 🧩 Component System

GameObject는 Component를 조합하여 기능을 확장하는 구조를 사용합니다.

### Core Components

- **Transform**
  - Position / Rotation / Scale 관리
  - World Matrix 생성

- **MeshRenderer**
  - Mesh + Material 바인딩
  - Renderer에 Draw 요청

- **Rigidbody**
  - Velocity / Acceleration 기반 물리 갱신
  - DeltaTime 기반 Euler Integration

- **Collider**
  - AABB / Sphere Collision
  - 충돌 시 기본 반응 처리 (정지 / 반사)

이 구조를 통해 **상속 폭발을 방지**하고,  
런타임 조합을 통한 유연한 오브젝트 구성이 가능합니다.

---

## 📦 Resource Management

- Mesh, Shader, Material은 **중복 로딩 방지를 고려한 구조**로 설계
- Material은 Shader / Texture / Constant Buffer를 하나의 렌더링 단위로 관리
- MeshLoader는 OBJ / FBX 등 외부 포맷 확장을 고려한 인터페이스 형태로 구현

---

## ⚙️ Physics & Collision (Basic)

- Rigidbody
  - Velocity 기반 이동
  - Gravity 적용 가능
- Collider
  - AABB 기반 충돌 검사
  - 단순 충돌 반응 처리

복잡한 물리 시뮬레이션보다는  
**엔진 구조 검증을 위한 최소 기능 구현**에 초점을 맞췄습니다.

---

## 🛠️ Implementation Notes

- `std::unique_ptr`, `std::shared_ptr`를 활용한 명확한 Ownership 관리
- DeltaTime 기반 Update로 Frame-rate Independent 동작 보장
- Renderer / Component / Resource 간 책임 분리
- 향후 멀티스레드 렌더링 확장을 고려한 구조 유지

---

## 🔮 Future Work (Planned Extensions)

- Lighting Model (Blinn-Phong → PBR)
- Normal Mapping
- Frustum Culling
- Resource Hot-Reload
- Multi-threaded Rendering Preparation

Lighting 모델은  
렌더링 파이프라인 안정화 이후 단계에서 구현할 예정입니다.

---

## 📌 Summary

본 프로젝트는  
**DirectX11 기반 3D 엔진의 구조적 이해와 시스템 설계 능력을 검증하기 위한 개인 엔진 프로젝트**입니다.

렌더링, 컴포넌트, 리소스 관리의 기본 뼈대를 직접 구현함으로써  
실제 엔진 개발에 필요한 핵심 개념을 학습하고 적용하는 데 목적이 있습니다.
