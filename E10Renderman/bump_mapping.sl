// simple renderman shader for texturing Mars
// by Daniel Jungblut, IWR Heidelberg, May 2008, daniel.jungblut@iwr.uni-heidelberg.de
// updated by Susanne Kroemker, IWR Heidelberg, May 2014 -- susanne.kroemker@iwr.uni-heidelberg.de


// renderman shader are similar to C in syntax
// there are various global variables to be used, see for example
// http://www.3delight.com/en/uploads/docs/3delight/3delight_39.html#SEC80

// on the 3Delight homepage you find a good reference for renderman shader programming
// http://www.3delight.com/en/uploads/docs/3delight/3delight_31.html#SEC72

surface bump_mapping(string texture_map = ""; string bump_map = ""; float exponent = 0.3) {

 // normalize the surface normal
 normal Nf = normalize(N);

 // read in color information from texture 
 color Ct = color texture(texture_map, s, t);

 // to implement bump mappings look for the funktion "calculatenormal"

 // here you may place your bump mapping
  
  float displacement = exponent * float texture(bump_map, s, t);
 // P += displacement * normalize (N);     <- bump with displacement
  N = calculatenormal (P + displacement * normalize (N));
 
// light calculations

 // normalize the incoming light
 vector V = normalize(I);
 // calculate the orientation of a face
 Nf = -faceforward(normalize(N), I);
 // transfer the opacity
 Oi = Os;
 // ambient part
 Ci = Oi * Ct * ambient();
 // specular part
 Ci += Oi * Ct * diffuse(Nf);
 // diffuse part
 Ci += Oi * Ct * specular(Nf, V, exponent);

}


