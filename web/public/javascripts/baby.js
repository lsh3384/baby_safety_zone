// ----------------비디오 스트리밍 나오는 canvas 설정----------------------
var canvas = document.getElementById("stream_out");
var ctx = canvas.getContext("2d");
ctx.fillStyle = "#FF0000";
var image2 = new Image();
image2.onload = function() {
	ctx.drawImage(image2, 0, 0);
};


// 소켓 연결-------------------------------------
const socket = io.connect('http://localhost:3000');

// 소켓 이벤트 bind ----------------------------
socket.on('image', (image) => {
	image2.src = 'data:image/jpg;base64,'+image;
});

socket.on('baby_out', (out) => {
	alert('Baby is out of safety zone!');
});


// 좌표 관련 전역변수-----------------------------------------------------
var coordinates = "";   // 웹서버로 보낼 좌표 배열 선언
var coordinates_html = []; // html에 표시하기 위한 좌표 설정

// canvas에서 마우스 이벤트 확인하고 좌표 받는 함수 ----------------------
function action_coords(event) {        
	var x = event.offsetX;  	// canvas기준으로 x좌표
	var y = event.offsetY;  	// canvas기준으로 y좌표
	
	coordinates += x + " ";
	coordinates += y + " ";	

	var coords = [];			// x,y좌표 쌍으로 넣을 배열
	coords.push(x);				// coords 배열에 x좌표추가
	coords.push(y);				// coords 배열에 y좌표추가
	coordinates_html.push(coords);	// coords 배열을 전역변수인 coordinates에 추가
	
	// coords_out div태그에 나타날 내용
	var coords_string ="";  	 // string변수 선언
	for (var i=0;i < coordinates_html.length;i++)	// coordinates에 들어있는 좌표로 coords_out div태그에 들어갈 내용 담기
		coords_string += "X" +(i+1) + ": " + coordinates_html[i][0] + ", Y" + (i+1)+ ": " + coordinates_html[i][1] + "<br/>";

	document.getElementById("coords_out").innerHTML = coords_string;
	// canvas 위에 좌표에 해당하는 위치에 네모칸 
	ctx.fillRect(x-10, y-10, 10, 10);
}


// 범위지정 버튼 클릭 연결 함수------------------------------------------------------------ 
function img_box_start() {
	coordinates = "";
	coordinates_html = [];
	ctx.clearRect(0, 0, 320, 240);
	socket.emit('resetFromHtml', "reset");
	document.getElementById("stream_out").setAttribute('onclick', 'action_coords(event)');
	document.getElementById("coords_out").innerHTML = '좌표 지정을 시작합니다.'; 
}

			
// 시작 버튼 연결 함수------------------------------------------------------------
function send_coords() {
	document.getElementById("stream_out").setAttribute('onclick', 'null');
	document.getElementById("coords_out").innerHTML = '좌표 지정이 완료 되었습니다.</br> 인식을 시작합니다.'; 
	socket.emit('coordinates', coordinates);
}


// 깜빡이는 함수-----------------------------------------------------------
function flashit(){
	var crosstable=document.getElementById("check_btn");
	if (crosstable){
		if (crosstable.style.backgroundColor.indexOf("rgb(12, 31, 51)")!=-1)
			crosstable.style.backgroundColor="#077af4";
		else 
			crosstable.style.backgroundColor="rgb(12, 31, 51)";
		}
}

var speed=500 ;   //속도 설정 1000 = 1초, 500 = 0.5초
//setInterval("flashit()", speed);