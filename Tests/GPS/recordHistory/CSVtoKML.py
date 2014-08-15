import csv
import simplekml
kml = simplekml.Kml()
#
#kml.save("botanicalgarden.kml")

inputFilename = 'E:\History.txt';
outputFilename = 'E:\History.kml';

print "Processing location data from " + inputFilename

with open('E:\History.txt', 'rb') as csvfile:
    csvReader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in csvReader:
        longitude = row[0];
        latitude = row[1];
        date = row[2];
        time = row[3];
        millis = row[4];
        description = "Date: " + date + "\nTime: " + time
        #print longitude + " by " + latitude + " on " + date + " at " + time + " since " + millis
        kml.newpoint(name=millis, description=description, coords=[(latitude,longitude)])

print "Done. File saved as " + outputFilename
kml.save(outputFilename)
