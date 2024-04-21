readChannelID = 2506706; 
TemperatureFieldID = 4;
IRVals = thingSpeakRead(readChannelID,'Fields',TemperatureFieldID, NumPoints=8000, ReadKey='50QMQKRPSTGYJ1UM');
histogram(IRVals); 
xlabel('Duration (Seconds)'); 
ylabel('Number of Measurements\newline for Each Duration'); 
title('Histogram of Door Opening Duration Variation');