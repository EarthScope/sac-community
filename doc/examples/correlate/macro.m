fg impulse npts 30 delta 0.1 begin -1.2
write imp.sac
cut 0 0.9
read imp.sac
write imp02.sac

fg impulse npts 30 delta 0.1 begin -0.8
write imp.sac
cut 0 0.9
read imp.sac
write imp06.sac

fg impulse npts 30 delta 0.1 begin -0.2
write imp.sac
cut 0 1.9
read imp.sac
write imp12.sac

cut off
read imp02.sac
ch b -1.0
write imp02n.sac

quit
