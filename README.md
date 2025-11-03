MyEngineW-DirectX11

Windows API 기반의 2D 엔진 구조에서 확장하여, DirectX11을 활용해 3D 렌더링 및 컴포넌트 기반 구조를 구현한 개인 게임엔진 프로젝트입니다.
현재 빛(lighting) 모델은 구현되어 있지 않으며, 향후 확장 기능으로 남겨두었습니다.

🚀 프로젝트 개요

언어 / 기술 스택: C++ (Modern C++ 기준), Windows API, DirectX11, HLSL(Shader)

개발 환경: Visual Studio (Windows), Windows 10 이상

출발점: 기존 Window API 기반 2D 엔진 구조

목표:

3D 렌더링 구조 이해 및 구현

컴포넌트 기반 아키텍처 설계

렌더러, 메시 로딩, 기본 물리/충돌 구현

빛(Light) 모델 없이 엔진의 기본 뼈대 구축

기능 범위: Mesh Rendering, Transform/Collider/Rigidbody 등 기본 컴포넌트, 카메라 제어, 스크립터블 씬 관리 등

🏗️ 아키텍처 및 디렉토리 구조
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

🔍 주요 기능 설명
Device & Renderer 초기화

Direct3D11 Device, DeviceContext, SwapChain, RenderTargetView, DepthStencilView 등 필수 객체를 생성

프레임마다 Clear → Draw → Present 루틴을 구현

Viewport 크기 변경 대응 및 리사이즈 처리

컴포넌트 기반 구조 설계

GameObject 또는 유사 구조체에 다양한 Component를 붙여 기능을 확장

대표 컴포넌트:

Transform: 객체의 위치, 회전, 스케일 상태

MeshRenderer: Mesh와 Material을 바인딩해 실제 렌더링

Rigidbody: 단순 물리, 속도/가속도 적용

Collider: AABB 또는 구형 충돌체로 충돌 감지

매니저 계층: 컴포넌트 매니저, 씬 매니저 등을 통해 생명주기 관리

렌더링 & 카메라

카메라는 View 및 Projection 행렬을 계산해 Shader에 전달

자유 시점(Free-look) 및 WASD + 마우스 입력으로 이동/회전 지원

메시와 머티리얼을 활용해 3D 객체 렌더링 가능

메쉬 리소스 및 머티리얼

메시 로더: OBJ, FBX 등 외부 에셋 포맷 지원 가능하도록 설계 (Assimp 사용 가능)

Material: 셰이더 프로그램, 텍스처, 상수버퍼 등 렌더링 상태 묶음

리소스 캐시 또는 리소스 매니저 구조로 중복 로딩 최소화

물리 및 충돌 간단 구현

Rigidbody는 Euler Integration 방식으로 속도와 위치 업데이트

Collider는 AABB 기반 충돌 체크. 충돌 시 속도 반사 또는 정지 처리를 간단히 구현

게임 오브젝트 간의 충돌 관계가 발생했을 때 기본 반응 처리

최적화 및 구조 개선 포인트

Smart Pointer (std::unique_ptr, std::shared_ptr) 사용으로 메모리 누수 방지 및 소유권 명확화

Resource, Component, Mesh 등 반복적 구조는 템플릿 또는 상속 구조 활용

DeltaTime 기반 이동 및 물리 적용으로 프레임 독립적 동작 보장

필요 시 프레임 타이밍 측정을 위해 TimeManager 활용

빛(Light) 모델은 아직 구현되지 않았으며, 렌더링 구조 확장 여지가 남아있음
