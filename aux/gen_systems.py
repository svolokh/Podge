import json
import sys

# gen_all_systems.py <systems.json> <out.inl>
if len(sys.argv) < 3:
    sys.stderr.write('Missing arguments\n')
    exit(1)

systems_path = sys.argv[1]
out_path = sys.argv[2]

with open(out_path, 'w') as out, open(systems_path, 'r') as f:
    systems = json.loads(f.read())
    for inl in systems:
        out.write('#include <{}>\n'.format(inl))
