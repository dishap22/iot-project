readChannelID = <replace>; 
TemperatureFieldID = <replace>; 
IRVals = thingSpeakRead(readChannelID,'Fields',TemperatureFieldID, NumPoints=8000);
histogram(IRVals); 
xlabel('Duration (Seconds)'); 
ylabel('Number of Measurements\newline for Each Duration'); 
title('Histogram of Door Opening Duration Variation');