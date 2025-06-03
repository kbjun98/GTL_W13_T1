# GTL_W13_T1

# Clone시 주의사항
이 레포지토리에는 submodule이 포함되어있어, 일반적인 방식으로 clone하면, 컴파일할 때 정상적으로 컴파일되지 않습니다.

> 다음 명령어를 사용해서 Clone해 주세요
```shell
git clone --recursive <repository_url>
```

> 이미 Clone을 했다면 다음 명령어를 추가로 입력해 주세요
```shell
git submodule update --init --recursive
```

# 게임 설명

### 목표

"세상을 당근으로 물들이겠다"는 토끼들의 비밀스러운 계획을 막아야합니다.

토끼로 변장하여 증거를 수집하고, 그들의 사악한 정체를 밝혀내세요.

### 조작 키

* W, A, S, D: 이동
* Space: 점프
* 마우스 이동: 시야 회전
* 마우스 우클릭 홀드: 카메라 모드 전환
* 마우스 좌클릭: 촬영
* E, Q: 카메라 줌 인, 아웃
* Tab: 마우스 커서 표시
