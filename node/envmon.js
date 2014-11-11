var pinoccio = require('pinoccio'); // works in browserify and server side
var xively = require("xively");

// create xively client
var xivelyclient = new xively();

// create an api instance with default options
var api = pinoccio("<<pinoccio_key>>");// in the browser the token may be read from a pinoccio_id cookie,
var stream = api.sync();

xivelyclient.setKey("<<xively_api_key>>");

stream.on("data", function(data){
	var streamData = data.data;
	
	// check for our troop and scout
	if(streamData.troop == 18 && streamData.scout == 1){
		// check our stream
		if(streamData.value.type == 'env'){
			console.log("Light level is " + streamData.value.l + " lx" + ", Temperature is " + streamData.value.t + " °C");
			
			// Datastreams
			var dp = {
				"version":"1.0.0",
				"datastreams" : [
					{
						"id" : "temperature",
						"current_value" : streamData.value.t
					},
					{
						"id" : "light",
						"current_value" : streamData.value.l
					},
				]
			}
			
			// Send data to Xively
			xivelyclient.feed.new('<<feed_id>>', {
				data_point: dp,
				callback: function(e) { 
					console.log("Data posted to Xively");
				}
			}); 
		}
	}
});