# ---------------------------------------
# Custom exporter - Still in development
# ---------------------------------------

# DESCRIPTION: This script exports the active mesh object in Blender to a custom text file format.

import bpy

# Get the active object in Blender
obj = bpy.context.object

# Make sure it's a mesh
if obj.type == 'MESH':
    # Apply the mirror modifier (if exists)
    for modifier in obj.modifiers:
        if modifier.type == 'MIRROR':
            bpy.ops.object.modifier_apply(modifier=modifier.name)

    # Create a copy to avoid modifying the original mesh
    mesh = obj.data.copy()
    
    # Convert to triangles
    mesh.calc_loop_triangles()

    with open("C:/Users/fadul/Downloads/simpleSpaceShip1.txt", "w") as f:
        # Export vertices
        f.write(f"Vertices: {len(mesh.vertices)}\n")
        for v in mesh.vertices:
            # Convert coordinates (Blender -> JOJ)
            x = v.co.x
            y = v.co.z   # Blender Y -> JOJ Z
            z = -v.co.y  # Blender Z -> JOJ -Z
            f.write(f"v {x} {y} {z}\n")
        
        # Export normals (already interpolated if Auto Smooth is enabled)
        f.write(f"Normals: {len(mesh.vertices)}\n")
        for v in mesh.vertices:
            normal = v.normal  # This is the normal that Blender calculates
            x = normal.x
            y = normal.z   # Blender Y -> JOJ Z
            z = -normal.y  # Blender Z -> JOJ -Z
            f.write(f"vn {x} {y} {z}\n")

        # Export triangulated faces
        f.write(f"Faces: {len(mesh.loop_triangles)}\n")
        for tri in mesh.loop_triangles:
            f.write(f"f {tri.vertices[0]} {tri.vertices[1]} {tri.vertices[2]}\n")

print("Export completed (triangles)!")
