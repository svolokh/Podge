import sys

if len(sys.argv) < 2:
    sys.stderr.write("Usage: python {} <name>\n".format(sys.argv[0]));
    exit(1)

name = sys.argv[1]
name_upper = name.upper()
print \
"""#ifdef PODGE_SYSTEM_FWD

#ifndef PODGE_SYSTEM_{{name_upper}}_HPP_FWD
#define PODGE_SYSTEM_{{name_upper}}_HPP_FWD

namespace podge { namespace systems { namespace {{name}} {

/* TODO */

} } }

#endif

#else

#ifndef PODGE_SYSTEM_{{name_upper}}_HPP
#define PODGE_SYSTEM_{{name_upper}}_HPP

namespace podge { namespace systems { namespace {{name}} {

struct system : entity_system {
    const char *name() const {
        return "{{name}}";
    }

    /* TODO */
};
PODGE_REGISTER_SYSTEM(system);

} } }

#endif

#endif
""".replace('{{', '__leftbrace__').replace('}}', '__rightbrace__').replace('{', '{{').replace('}', '}}').replace('__leftbrace__', '{').replace('__rightbrace__', '}').format(**locals())
