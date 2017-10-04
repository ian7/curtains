/* loud Function.
 *
 * @param {Object} req Cloud Function request context.
 * @param {Object} res Cloud Function response context.
 */

const http = require('http')
const bodyParser = require('body-parser');
const express = require('express');
var mqtt = require('mqtt')
const fs = require('fs');
const jp = require('jsonpath')
const port = 3003

var client  = mqtt.connect('mqtt://10.10.1.3')

const requestHandler = function p(req, res) {
    response = "This is a sample response from your webhook!" //Default response from the webhook to show it's working

    res.setHeader('Content-Type', 'application/json'); //Requires application/json MIME type
    responseString = JSON.stringify({ "speech": response, "displayText": response, "source": "something", "data": "haha" });
    res.end(responseString);
    console.warn("request served");
    console.log(req);
    
};

const server = http.createServer(requestHandler)

var app = express();

app.post('/', function(request, res) {
    var body = '';
    filePath = 'request.json';
    request.on('data', function(data) {
        body += data;
    });
    response = "I'm too stupid for this" //Default response from the webhook to show it's working


        fs.appendFile(filePath, body, function() {
        var quote = jp.query(JSON.parse(body),"$.originalRequest.data.inputs.*.rawInputs.*.query")[0];

        console.log( quote );

        if( quote.indexOf("light") != -1 ){
            console.log('light');
            quote = quote.toLowerCase();

            if( quote.indexOf("on") != -1){
                  client.publish('/light/brightness', '255');
                  console.log('on');
                  response = "full power!";
            }
            if( quote.indexOf("dim") != -1){
                  client.publish('/light/brightness', '100');
                  response = "light is dimmed";
            }
            if( quote.indexOf("off") != -1){
                  client.publish('/light/brightness', '0');
                  response = "light is switched off";
            }
            if( quote.indexOf("red") != -1){
                  client.publish('/light/hue', '96');
                  response = "sexy";
            }
            if( quote.indexOf("blue") != -1){
                  client.publish('/light/hue', '154');
                  response = "oh so blue";
            }
            if( quote.indexOf("sunrise") != -1){
                  client.publish('/light/hue', '142');
                  response = "wake up";
            }
            if( quote.indexOf("ocean") != -1){
                  client.publish('/light/hue', '225');
                  response = "so salty";
            }
            if( quote.indexOf("yellow") != -1){
                  client.publish('/light/hue', '87');
                  response = "sunflower mode on";
            }
            if( quote.indexOf("white") != -1){
                  client.publish('/light/saturation', '0');
                  response = "simply white";
            }
            if( quote.indexOf("medium") != -1){
                  client.publish('/light/saturation', '128');
                  response = "bleak color";
            }
            if( quote.indexOf("strong") != -1){
                  client.publish('/light/saturation', '255');
                  response = "oh so colorfull";
            }
        }

    res.setHeader('Content-Type', 'application/json'); //Requires application/json MIME type
    responseString = JSON.stringify({ "speech": response, "displayText": response, "source": "something", "data": "haha" });
    res.end(responseString);

  
    request.on('end', function (){

        });
    });
});

app.listen(3003);

/*
app.use(function (req, res) {
  res.setHeader('Content-Type', 'text/plain')
  res.write('you posted:\n')
  res.end(JSON.stringify(req.body, null, 2))
})

*/


/*
server.listen(port, (err) => {
    if (err) {
        return console.log('something bad happened', err)
    }

    console.log(`server is listening on ${port}`)
})
*/