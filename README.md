# self-parking-car
This project was made during my bachelors degree.

In today's world many automobile OEM's are integrating more driver assist systems. This project was build to show the working prototype of a self parking car. The project assumes the scenario where the left side of the road will consist of walls and parking areas. 

The model will detect available space using 2 sensors on the left side of the car and 1 sensor each on the front and rear. When the car starts to move, the sensors on the left side continuously measure the distance and look for a suitable width for the parking area. If both sensors determine the appropriate distance for the parking, the parking process is started, else the car moves forward to look for other available space. After, the completion of parking sms is sent to the user on his mobile number, using gsm module.

# Project Image
![Project Image](https://github.com/atharvsharma1998/self-parking-car/blob/main/SideView.jpeg?raw=true)
