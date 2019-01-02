import json
import sys
import string

# gen_types <types.json> <out.inl>
if len(sys.argv) < 3:
    sys.stderr.write('Missing arguments\n')
    exit(1)

types_path = sys.argv[1]
out_path = sys.argv[2]

with open(out_path, 'w') as out, open(types_path, 'r') as f:
    types = json.loads(f.read())
    out.write('namespace podge { namespace types {\n')
    for k in types:
        v = types[k]
        systems = map(lambda s: '"{}"'.format(s), v['systems'])
        out.write('podge_registry_init PODGE_CONCAT(init_,, __COUNTER__)(podge_registry_init::type_tag(), [](podge_registry_init::register_type_fn r) {{ r("{}", {}, {{{}}}); }});\n'.format(k, 'true' if v['has_body'] else 'false', string.join(systems, ',')))
    out.write('} }\n')
