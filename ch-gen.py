# Example: `python ch-gen.py -f block -s src -i inc -n gil_block`

import ntpath
import optparse
import os

op = optparse.OptionParser()
op.add_option("-f", "--filename", action="store", type="string",
              dest="filename",
              help="File name to use for both the C and H files.")
op.add_option("-s", "--sourcebase", action="store", type="string",
              dest="sourcebase",
              help="Source file base path.")
op.add_option("-i", "--headerbase", action="store", type="string",
              dest="headerbase",
              help="Header file base path.")
op.add_option("-n", "--interfacename", action="store", type="string",
              dest="interfacename",
              help="Optional interface name. Uses file name if missing.")

opts, args = op.parse_args()

if not opts.filename:
  print("Missing file name (-f).")
  quit()

srcbase = "{}/".format(opts.sourcebase) if opts.sourcebase else ""
incbase = "{}/".format(opts.headerbase) if opts.headerbase else ""

cfile = "{0}{1}.c".format(srcbase, opts.filename)
hfile = "{0}{1}.h".format(incbase, opts.filename)

cpath = ntpath.dirname(cfile)
hpath = ntpath.dirname(hfile)

if cpath and not os.path.exists(cpath):
  os.makedirs(cpath)

if hpath and not os.path.exists(hpath):
  os.makedirs(hpath)

if ntpath.exists(cfile):
  print("{} exists already.".format(cfile))
  quit()

if ntpath.exists(hfile):
  print("{} exists already.".format(hfile))
  quit()

iface = opts.interfacename

if not iface:
  iface = ntpath.basename(opts.filename)

cinc = "#include \"{0}\"".format(hfile[len(incbase):])
cstr = "const struct {0} {1} = {{ }};".format(iface.upper(), iface.lower())

ccnt = "{}{}{}{}".format(cinc, os.linesep, os.linesep, cstr.replace("-", "_"))

hg = "{}_H".format(iface).upper()

hdef = "#ifndef {}\n#define {}\n\n".format(hg, hg)
hstr = "struct {} {{ }};\n\n".format(iface.upper())
hext = "extern const struct {} {};\n\n".format(iface.upper(), iface.lower())
hend = "#endif"

hcnt = "{}{}{}{}".format(hdef, hstr, hext, hend)
hcnt = hcnt.replace("-", "_")

c_file = open(cfile, "w+")
c_file.write(ccnt)
c_file.close()

h_file = open(hfile, "w+")
h_file.write(hcnt)
h_file.close()
