# 세부 기능 설명
- 간단한 설명 아래 링크를 통해 자세한 설명 확인

# 영상 전송
사용 라이브러리: base64
- OpenCV로 처리 된 영상을 jpg로 변환
- jpg형식의 파일을 문자형식인 base64로 인코딩
- 웹페이지에서 base64를 이용해서 영상 표시 가능
- https://treasure-sardine-f0f.notion.site/base64-6472cf87dc124b89915309865327f5ab


# 안전 범위 내 물체 판별
사용 라이브러리: boost
- 하나의 다각형이 다른 다각형에 포함되어있는지를 판별하여 true, false 반환하는 함수 사용
- https://treasure-sardine-f0f.notion.site/boost-dfdf7d5545834c48974c247c5bf5c60a

# 물체 인식
사용 라이브러리: OpenCV
사용 모델: YOLOv3
- OpenCV에서 제공하는 DNN모듈을 사용하여 YOLOv3 학습모델을 로딩
- 물체 인식 결과 반환값인 class와 좌표를 사용하여 영상에 물체 표시
- https://treasure-sardine-f0f.notion.site/YOLO-3749a06b303d4388a8524ef365532bd1

