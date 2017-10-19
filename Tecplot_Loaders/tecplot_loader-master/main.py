from tecplot_loader import *

mesh = parse("../meshes/title.dat")
print(mesh.get_title())
zone = mesh.get_zone(0)
print(zone.get_number_of_elements())
#print(mesh.get_value(0, "X"))
#print( mesh.get_number_of_elements() )
# if mesh.get_format() == Mesh.FEPOINT:
# 	print("it's a fepoint!")
