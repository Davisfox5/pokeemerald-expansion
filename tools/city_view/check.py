"""Fail the build when a sectioned city's native source changed without regenerating its view tables."""
from pathlib import Path
import hashlib, json, sys

root = Path(__file__).resolve().parents[2] if len(sys.argv) == 1 else Path(sys.argv[1])

def digest(p):
    data = p.read_bytes()
    if p.suffix == '.pal':
        data = data.replace(b'\r\n', b'\n')
    return hashlib.sha256(data).hexdigest()

stale = []
for manifest in sorted((root / 'data/city_views').glob('*/source-hashes.json')):
    for name, sha in json.loads(manifest.read_text()).items():
        p = root / name
        if not p.is_file() or digest(p) != sha:
            stale.append(name)
if stale:
    print('Sectioned city native source changed without regenerating its view tables:', file=sys.stderr)
    for name in stale:
        print('  ' + name, file=sys.stderr)
    print('Regenerate with the city\'s stream.py from the Pokemon Apocrypha hub (see tools/city_view/README.md),', file=sys.stderr)
    print('then commit the native edits and the regenerated tables together.', file=sys.stderr)
    raise SystemExit(1)
print('City view tables agree with their native sections.')
