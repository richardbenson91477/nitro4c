# tested with Blender 2.69
import bpy

fname_s = "output.splinea"
step_t = 1.0

def main ():
    f_out = open (fname_s, "wb")
    if not f_out:
        return -1

    bp = bpy.data.curves[0].splines[0].bezier_points
    bpn = len (bp)
    if not bpn:
        return -1

    f_out.write (("3\nSPLINE_TYPE_CUBIC_BEZIER\n\n%u\n\n" % bpn).encode())

    t = 0.0
    for b in bp:
        # swap -y and z
        f_out.write (("%f\n%f %f %f\n%f %f %f\n%f %f %f\n\n" % (t, \
            b.co[0], b.co[2], -b.co[1], \
            b.handle_right[0], b.handle_right[2], -b.handle_right[1], \
            b.handle_left[0], b.handle_left[2], -b.handle_left[1])).encode ())
        t += step_t

    f_out.close ()
    del f_out

if __name__ == "__main__":
    main ()
