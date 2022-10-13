# ESP32 controlled Hörmann HSE 4 BS remote

## Problem 

In front of my house, the new owner renovated an old building into a new multi-apartment facility, and we also got a new parking lot with a nice fancy Hörmann parking gate. Our household was only given two remotes (Hörmann HSE 4 BS) that could open the gate. That was a problem because we own three cars and five people live in the house. Therefore, it was quite a logistic challenge to know who has the remote with him and is in range to open the gate. Also, this was an unpleasant experience for our visitors who had to wait in front of the gate. Then had to call us before they could enter. After a while, another problem occurred. The battery in the remote got a bit drained and the range of the remote went from 50 meters to around 25 meters. This meant that you had to walk outside the house closer to the gate to open it. Real pain in the rain to quickly open the gate while your visitor is waiting on the street.

## Ideas
The simple solution would be to buy more remotes, and that’s it. But this still wouldn’t fix the issue of remote range. By the way, the remotes cost more than 50€, which is not cheap if I want to give one to all my friends 😊

Hörmann also sells official modules that could upgrade the gate. For example, a mobile module, so the gate would open if you called a specified phone number or a Wi-fi module… But I am not the owner of the Hörmann gate, so I cannot buy these modules and upgrade the gate. 

So, I got an idea. What if I could put my Hörmann remote in a position (inside my house) where it would be in range to open the gate, and then remotely trigger that remote to open the gate?

## The process of making my remote control remote controlled ;)
I started exploring. My goal was to figure out how to trigger the buttons on the remote without clicking the buttons. Firstly I opened up the remote and checked the circuit.  
![Hörmann HSE 4 BS remote circuit](./photos/remote-circuit.JPG "Hörmann HSE 4 BS remote circuit")

On the circuit, I found four buttons that can open the gate (buttons need to be pre-programmed with the master remote). These are normal [push buttons](https://components101.com/switches/push-button) which means we can simulate the button press if we short the connection between the left and the right button terminals (T1 and T2).  
![Short button terminals](./photos/TerminalsButton.png "Short button terminals")

![How the button works](./photos/Push-button-Pinout.gif "How the button works")  
[Gif Source 2022](https://components101.com/switches/push-button)

Now that we know how to simulate the button press, we need to choose a method to do it. There are two ways to accomplish this. The first is with relays, and the second is with transistors. I chose the relay method because it is simple and intuitive. You just wire it up in parallel and the original button still works but now you can trigger it with a relay. The only downside is that it is a bit louder because relays make this clicking sound when they are triggered. The method with transistors would be silent but we would need to figure out how the buttons are wired up and that requires more electrical knowledge than I have :D 

Now we have to figure out where to solder the wires on this small circuit. I started tracing circuit traces with a digital multimeter. My intention was to find the most suitable solder spots for the wires on the remote circuit. I also found the soldering points for the power so we can remove the battery and always have a "full charge" by powering the remote from an external source. The solder points are chosen in a way so they change the remote as little as possible. We can just cut off the wires and the remote should work as usual.

![Solder points](./photos/Solder%20points.png "Solder points")

Because the components are small and I am not capable of soldering such small components, I gave the remote circuit to my friend, who can do that. He soldered the wires on the agreed spots. This is what he gave back to me. He also installed the USB B connector with a chip that outputs 5V and 3V so we can ditch the battery.    
![Soldered remote with wires](./photos/solderedRemote.png "Soldered remote with wires")  

Now if we plug in the USB with power and short these two pins for a second it is the same as pressing the button. No range problems when powering it from USB :)
![Short Pins](./photos/shortPins.png "Short pins")

## solution



## Prerequisites


# Credits
