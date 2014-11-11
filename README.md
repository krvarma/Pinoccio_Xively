Pinoccio and Xively Integration
-------------------------------

Here is a project I worked last weekend, integrating Pinoccio and Xively. The application uses Pinnoccio, TSL2561 Light Sensor and TMP102 Temperature Sensor. There are two modules, first one is the Pinoccio application that retrieve the Light and Temperature sensor values and post to Pinoccio HQ. The second module is the Node.js module to subscribe to the live Pinoccio streams and post the sensor data to Xively. To connect and send data to Xivel, I am using [this library](https://github.com/nazt/node-xively). You can log on to Xively to visualize the sensor values.

**Screenshots**

![Xively Dashboard](https://raw.githubusercontent.com/krvarma/Pinoccio_Xively/master/screenshots/dashboard.png)

![Pinoccio](https://raw.githubusercontent.com/krvarma/Pinoccio_Xively/master/screenshots/pinoccio.JPG)