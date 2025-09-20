# viteTest_version2

**컴퓨터공학 전공 과제(Project Assignment)** 로 진행한 텍스트 버퍼 실험 프로젝트입니다.  
version1에서 큰 데이터를 읽으면 프로그램이 멈추는 문제를 해결하기 위해, **청크(Chunk) 기반 버퍼 구조**를 도입한 개선 버전입니다.

---

## 🔄 버전 히스토리

- **Version1**
  - 단순 연속 버퍼 방식
  - 큰 파일을 읽으면 메모리 과부하로 프로그램이 멈추는 오류 발생

- **Version2 (현재 버전)**
  - Chunk 기반 버퍼 구조 적용
  - 대용량 파일 안정적 처리 가능
  - 모듈별 코드 정리 및 테스트 데이터 추가

---

## 🎯 과제 목표

- 대용량 텍스트 파일 처리 문제 해결
- Chunk 기반 메모리 관리 기법 학습
- 삽입/삭제 시 전체 복사 최소화
- 코드 모듈화 (입출력, UI, 단어 처리)

---

## ✨ 주요 기능

- Chunked Buffer 설계
- 빈 청크 재사용 (프리 리스트)
- 파일 입출력 모듈 (`io.*`)
- 단어 처리 모듈 (`wordlist.*`)
- Windows 콘솔 UI (`ui_windows.c`)
- 샘플 테스트 데이터 제공 (`test/`)

<img width="732" height="1114" alt="image" src="https://github.com/user-attachments/assets/bcb637d2-bd71-4538-8da7-ebf801969ccf" />
<img width="1164" height="597" alt="image" src="https://github.com/user-attachments/assets/5887e9a1-f262-4a5e-aec2-fb4f1d6da4f1" />
<img width="1090" height="558" alt="image" src="https://github.com/user-attachments/assets/4bf5a395-a56a-472a-a219-2df14f80b4c2" />

---

## 📁 프로젝트 구조

```
viteTest_version2/
├─ main.c
├─ io.c / io.h
├─ ui_windows.c / ui.h
├─ wordlist.c / wordlist.h
├─ test/
├─ viteTest.sln / viteTest.vcxproj*
└─ .gitignore / .gitattributes
```

---

## 🚀 빌드 & 실행

### Visual Studio
1. `viteTest.sln` 열기
2. 빌드 후 실행

### MinGW / GCC
```bash
gcc -O2 -std=c11 main.c io.c ui_windows.c wordlist.c -o viteTest.exe
.ViteTest.exe
```

---

## 🧪 사용 방법

1. 프로그램 실행
2. 파일 열기
3. 편집 및 검색
4. 저장 후 종료

---

## 📚 배운 점

- 버전 관리 경험 (문제 → 원인 → 해결)
- Chunk 기반 버퍼 구조의 장점 체감
- C 언어 메모리 제어 (malloc/free) 숙련
- 코드 모듈화 및 유지보수성 강화
