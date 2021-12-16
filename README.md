# 프로젝트명

Baby Safety Zone

# 프로젝트 팀 구성원

- 이성현(PM)

- 최강훈

- 김중민

# 프로젝트 개요

- 보호자가 잠시 자리를 비운 사이에 아이들이 안전범위를 벗어날 경우 안전 사고 발생 가능성이 증가하게 됩니다. 실제 8개월 된 아들을 키우는 아빠의 입장에서, 아이가 매트를 벗어나서 머리를 바닥에 찧는 경우를 보고 이 프로젝트 주제를 선정하게 되었습니다.

- Baby Safety Zone의 주요기능은 아이가 안전 범위를 벗어났을 경우 보호자에게 푸시메시지로 위험 알림을 주는 기능입니다.

# 개발 환경

- 언어: C/C++, Javascript, Kotlin

- OS:  Ubuntu

- Library: OpenCV, boost, socket.io, base64

- Tools: vscode, android studio, Cmake

- Framework: node.js, YOLOv3

- 배포환경: GCP


# HW 구성

- Jetson Nano

- USB CAM


# SW 구성

- Main Function

  - 안전범위 지정: 클릭 이벤트로 안전범위 좌표 획득 → 영상에 안전 범위 표시 

  - 안전범위 지정: 클릭 이벤트로 안전범위 좌표 획득 → 영상에 안전 범위 표시 

  - 안전범위 이탈 판별: 안전범위로 지정된 다각형에 인식된 물체의 사각형 포함여부 판별 

  - define filepath : filesystem

- Sub Function

  - 안전범위 이탈시 푸시메시지 알림: andriod studio and FCM



- Data Communitation
  
  - socket.io
    
  - server : nodejs

  
# 프로젝트 작업 과정

### 소요기간 약 6주

- 프로젝트 주제 정하기

- 프로젝트 기능 정하기

- Python으로 Prototype 구현하기(아래 링크 참조)  
  https://github.com/lsh3384/baby_safety_zone_prototype.git

- C++로 변경하여 구현하기

- 부가 기능 구현하기


# 외관 

![dwd](https://github.com/lsh3384/baby_safety_zone/blob/master/exterior.jpeg?raw=true)

# 동작영상

### (이미지 클릭후 영상으로 이동)
<br/>

[![Video Label](https://github.com/lsh3384/baby_safety_zone/blob/master/phone.png?raw=true)](https://www.youtube.com/watch?v=X8SZp3SYpKg)


# FLOWCHART
## 안전범위 설정 과정

![dwdwd](https://github.com/lsh3384/baby_safety_zone/blob/master/process_setting_%20coordinates.png?raw=true)


## 물체 인식 및 안전범위 판별 과정

![dwdwd](https://github.com/lsh3384/baby_safety_zone/blob/master/process_start_recognition.png?raw=true)


# 코드 리뷰 (Opencv)

<!-- ![frfef](https://user-images.githubusercontent.com/88933098/142838223-159f460b-5c71-40a5-80d5-3b9ea32e0597.png) -->

<!-- ## 1. find pet object : matchTemplate

![1](https://user-images.githubusercontent.com/88933098/142357606-de9da55c-f217-4e12-8ecc-d00a86a5258c.JPG)

~~~
//읽어온 이미지와 datasheet sample image를 비교하는 부분
for(int i = 0; i < SAMPLENUM; i++){
  	matchTemplate(subImg_, matReadVector[i], resVecotr[i], TM_CCOEFF_NORMED);
	normalize(resVecotr[i], res_normVector[i], 0, 255, NORM_MINMAX, CV_8U);
	minMaxLoc(resVecotr[i], 0, &maxvVector[i], 0, &maxlocVector[i]);
}
//유사도를 비교하는 부분
if(maxvVector[0] > 0.7){
	serialPort1.Write("N");
	cout << "CAP" <<endl;
	break;
}
 ~~~ -->