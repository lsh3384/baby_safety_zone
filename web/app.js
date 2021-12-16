const path = require('path');
const express = require('express');
const fs = require('fs');
const app = express();
const server = require('http').Server(app);
const io = require('socket.io')(server);

// firbase admin setting
// var admin = require("firebase-admin");
// var serviceAccount = require("./serviceAccountKey.json");
// let registrationToken = 'f10VtzzJQy2J6HU-l6DQYQ:APA91bHR_ivK-zATXVGEBNfzf9YEaExtduWvzmxk73PR4murY53XZuXbtDxfSZMyA1mEKzTGyjih__P_quyBx_t5r_iLSrs3xE6llj9vlEoVD9ZT1bQxuNKSjsFJ0wf9XYqEihjSE5JE';

// admin.initializeApp({
//     credential: admin.credential.cert(serviceAccount),
//     //databaseURL: "https://javamsgs-5da73-default-rtdb.firebaseio.com"
// });

// static Æú´õ À§Ä¡ ÁöÁ¤
app.use(express.static(__dirname + '/public'))

// ÁÖ¼Ò(http://localhost:3000)¿¡ /¸¦ ºÙ¿©¼­ get¹æ½ÄÀ¸·Î ¿äÃ»ÇßÀ» ¶§ ½ÇÇàÇÒ ³»¿ë Á¤ÀÇ
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, './public/index.html'));
});

// socket.io¸¦ »ç¿ëÇØ¼­ ¼ÒÄÏ¿¡ µé¾î¿À´Â connection ÀÌº¥Æ® ´ë±â
io.on('connection', function(socket) {  // connectionÀÌ ÀÌ·ç¾îÁö¸é »ó´ë¹æ°ú ¿¬°áµÈ socketÀ» ¹ÝÈ¯>$
  console.log('A new connection was made.');

  // connectionÀÌ ÀÌ·ç¾îÁø socket¿¡¼­ µé¾î¿Ã dataÀÌº¥Æ® ´ë±â ¹× dataÀÌº¥Æ® ¹ß»ý½Ã ½ÇÇàÇÒ ³»¿ë
  socket.on('image', function(data){
    //console.log('data received');
    //console.log(data);
    io.emit('image', data.toString());  //image ÀÌº¥Æ®¸¦ ¹ß»ý
  });

  // coordinate ÀÌº¥Æ® ´ë±â ¹× ½ÇÇà ³»¿ë
  socket.on('coordinates', function(coords) {
    console.log("Sending coordinates to the cpp client.");
    io.emit('coordinates', coords);    // coordinates ÀÌº¥Æ®¸¦ ¹ß»ý
        //io.emit('coordinates', "12 34 56 78");
        //io.emit('coordinates', "hello");
  });

    // coordinate ÀÌº¥Æ® ´ë±â ¹× ½ÇÇà ³»¿ë
    socket.on('baby_out', function(out) {
      console.log("Baby out event occurred.");
      io.emit('baby_out', out);           // baby_out ÀÌº¥Æ®¸¦ ¹ß»ý
          let payload = {
                  notification: {
                          title: "notification",
                          body: "Baby out of safety zone!"
                  }
          };
          admin.messaging().sendToDevice(registrationToken, payload)
           .then((response) => {
           // See the MessagingDevicesResponse reference documentation for
           // the contents of response.
           console.log('Successfully sent message:', response);
           })
          .catch((error) => {
                   console.log('Error sending message:', error);
           });
    });

  // reset ÀÌº¥Æ® ´ë±â ¹× ½ÇÇà ³»¿ë
  socket.on('resetFromHtml', function(reset) {
    console.log("Boundary reset request was made.");
    io.emit('resetFromHtml', reset);
  });
});

server.listen(3001);
