/* loud Function.
 *
 * @param {Object} req Cloud Function request context.
 * @param {Object} res Cloud Function response context.
 */

const http = require('http')
const bodyParser = require('body-parser');
const express = require('express');
const fs = require('fs');
const jp = require('jsonpath')
const port = 3003

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
    filePath = 'ata.txt';
    request.on('data', function(data) {
        body += data;
    });
    response = "This is a sample response from your webhook!" //Default response from the webhook to show it's working

    res.setHeader('Content-Type', 'application/json'); //Requires application/json MIME type
    responseString = JSON.stringify({ "speech": response, "displayText": response, "source": "something", "data": "haha" });
    res.end(responseString);


    request.on('end', function (){
        fs.appendFile(filePath, body, function() {
        console.log(jp.query(JSON.parse(body),"$.originalRequest.data.inputs.*.rawInputs.*.query"));
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