camera {
	location <0, 0, 14>
	up <0, 1, 0>
	right <1.3333, 0, 0>
	look_at <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

//little
sphere { <2, 0, -5>, 2.01
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4}
}

//medium
sphere { <0, 0, -3.5>, 2.25
  pigment { color rgb <0.15, 0.35, 0.2>}
  finish {ambient 0.2 diffuse 0.4}
}

//big
sphere { <2, 0, -10.5>, 3.75
  pigment { color rgb <0.25, 0.25, 0.25>}
  finish {ambient 0.2 diffuse 0.4}
}

//left
sphere { <-4.5, 1, 0>, 1.1
  pigment { color rgb <1, 1, 1>}
  finish {ambient 0.2 diffuse 0.4}
}
