float haversine(float flat1, float flon1, float flat2, float flon2)
{
  // Source: http://www.movable-type.co.uk/scripts/latlong.html
  
  // Formula: 
  // distance = c * r
  // R = radius of earth
  // c = 2 * atan2(sqrt(a),sqrt(1-a))
  // a = a1^2 + a2a*a2c*a2c^2
  // a1 = sin(dPhi/2)
  // a2a = cos(Phi1)
  // a2b = cos(Phi2);
  // a2c = sin^2(dPsi/2)
  // dPhi = Phi2 - Phi1
  // Phi = lat in radians
  // Psi = long in radians
  
  float a=0;
  float a1=0;
  float a2=0;
  float c=0;
  
  float dPhi = radians(flat2-flat1);
  float Phi1 = radians(flat1);
  float Phi2 = radians(flat2);
  float dPsi = radians((flon2)-(flon1));
  
  a1 = (sin(dPhi/2.0) * sin(dPhi/2.0));  // a1^2
  a2 = cos(Phi1); // a2a 
  a2 *= cos(Phi2); // a2a*a2b
  a2 *= sin(dPsi/2.0); // a2a*a2b*a2c
  a2 *= sin(dPsi/2.0); // a2a*a2b*a2c^2
  a = a1 + a2; // a
  
  c = (2*atan2(sqrt(a),sqrt(1.0-a)));
  
  return c * 12756200; // distance = c * r. Return distance in meters * 2 
}

void setup()
{
  print(haversine(30.324471, -81.586759, 30.326809, -81.588632));
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}


