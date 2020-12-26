import pyclipper
import numpy as np

subj = [253,285,824,296,833,435,283,413]
clipp = [215,267,853,296,826,449,246,414]

subj = np.array(subj, dtype=np.float)
clipp = np.array(clipp, dtype=np.float)

subj = subj.reshape((4, 2))
clipp = clipp.reshape((4, 2))


pc = pyclipper.Pyclipper()
invalid = False
try:
    bool_subj = pc.AddPath(subj, pyclipper.PT_SUBJECT, True)
    bool_clipp = pc.AddPath(clipp, pyclipper.PT_CLIP, True)
except pyclipper.ClipperException as e:
    invalid = True
if not invalid:
    inter_path = pc.Execute(pyclipper.CT_INTERSECTION, pyclipper.PFT_EVENODD)
    union_path = pc.Execute(pyclipper.CT_UNION, pyclipper.PFT_EVENODD)

    inter = pyclipper.Area(inter_path[0]) if len(inter_path) > 0 else .0
    uunion = pyclipper.Area(union_path[0]) if len(union_path) > 0 else .0
    ovr = inter / max(uunion, 1.)  # 防止0除溢出
else:
    ovr = .0
pc.Clear()

print("IoU: {:3f}".format(ovr))