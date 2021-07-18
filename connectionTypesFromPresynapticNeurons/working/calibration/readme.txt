
//calibration/readme.txt
These files are used to calibrate the connectionTypesDerivedFromPresynapticNeurons/raw/in_body_cell_connection.csv [svg generated] coordinates with respect to the connections_I/E.svg coordinates.

---
connectionTypesDerivedFromEMimages/connections_IE1-calibrationColours.svg
connectionTypesDerivedFromPresynapticNeurons/connections_IE2-roughCalibration-calibrationColours.svg

Mark neurons in svg GUI (inkscape) such that they can be identified in svg markup. Fill colours for calibration:
left bottom neuron #1 - D45500
top right neuron #2 - FF6600


connectionTypesDerivedFromEMimages/connections_IE1-calibrationColours.svg:
	left bottom neuron:
          <use
             style="fill:#d45500;stroke:url(#exciteNeuronColor)"
             x="2014.09992"
             xlink:href="#m51ca76320d"
             y="2305.61904"
             id="use53468" />
	top right neuron:
          <use
             style="fill:#ff6600;stroke:url(#exciteNeuronColor)"
             x="2323.34352"
             xlink:href="#m51ca76320d"
             y="2194.6068"
             id="use72118" />
			 
connectionTypesDerivedFromPresynapticNeurons/connections_IE2-roughCalibration-calibrationColours.svg / connections_IE2-noCalibration.svg [roughCalibration/noCalibration]:
	left bottom neuron:
		  <use
             style="fill:#d45500;stroke:url(#inhibitNeuronColor)"
             x="2802.0137450216"
             xlink:href="#m51ca76320d"
             y="2644.34736"
             id="use49508" />
          L24777;
		  	connections_IE2-roughCalibration.svg;
    			<use style="fill:url(#inhibitNeuronColor);stroke:url(#inhibitNeuronColor);" x="2802.0137450216" xlink:href="#m51ca76320d" y="2644.34736"/>
     		connections_IE2-noCalibration.svg;
				<use style='fill:url(#inhibitNeuronColor);stroke:url(#inhibitNeuronColor);' x='1932.93' xlink:href='#m51ca76320d' y='1517.66'/>
	top right neuron:
		L17654;
    		 <use style="fill:url(#inhibitNeuronColor);stroke:url(#inhibitNeuronColor);" x="3108.95416604369" xlink:href="#m51ca76320d" y="2533.33512"/>
          <use
             style="fill:#ff6600;stroke:url(#inhibitNeuronColor)"
             x="3108.95416604369"
             xlink:href="#m51ca76320d"
             y="2533.33512"
             id="use35316" />
          L24777;
		  	connections_IE2-roughCalibration.svg;
    			 <use style="fill:url(#inhibitNeuronColor);stroke:url(#inhibitNeuronColor);" x="3108.95416604369" xlink:href="#m51ca76320d" y="2533.33512"/>
     		connections_IE2-noCalibration.svg;
     			<use style='fill:url(#inhibitNeuronColor);stroke:url(#inhibitNeuronColor);' x='2348.58' xlink:href='#m51ca76320d' y='1368.45'/>

			 			 
so to transform connectionTypesDerivedFromPresynapticNeurons/connections_IE2-noCalibration.svg coordinates to connectionTypesDerivedFromEMimages/connections_IE1.svg coordinates, require;

neuron #1: x1=1932.93, y1=1517.66
neuron #2: x2=2348.58, y2=1368.45
->
neuron #1: x3=2014.09992, y3=2305.61904
neuron #2: x4=2323.34352, y4=2194.6068

x1=1932.93
y1=1517.66
x2=2348.58
y2=1368.45
x3=2014.09992 
y3=2305.61904
x4=2323.34352
y4=2194.6068
	
2 simultaneous equations to solve;			 

(1)
x1*q1 + q2 = x3
x2*q1 + q2 = x4

(2)
y1*r1 + r2 = y3
y2*r1 + r2 = y4

(1)
x1*q1 + q2 = x3
x2*q1 + q2 = x4
x1*q1 + x2*q1 + 2q2 = x3 + x4
q1*(x1 + x2) + 2q2 = x3 + x4
q1*(x1 - x2) = x3 - x4
q1 = (x3 - x4)/(x1 - x2)
q2 = ((x3 + x4) - q1*(x1 + x2))/2

(2)
y1*r1 + r2 = y3
y2*r1 + r2 = y4
r1*(y1 + y2) + 2r2 = y3 + y4
r1*(y1 - y2) = y3 - y4
r1 = (y3 - y4)/(y1 - y2)
r2 = ((y3 + y4) - r1*(y1 + y2))/2

q1 = 0.744  
q2 = 576.00  
r1 = 0.744  
r2 = 1176.48 

[with *0.01]

---
scilab online
https://cloud.scilab.in/
scilab online increase precision
https://groups.google.com/g/comp.soft-sys.math.scilab/c/HaMESPnfRjw?pli=1


	
	
