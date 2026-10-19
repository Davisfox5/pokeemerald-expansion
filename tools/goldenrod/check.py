"""Fail a production build if native Goldenrod edits have a stale view atlas."""
from pathlib import Path
import hashlib,json,sys
root=Path(__file__).resolve().parents[2] if len(sys.argv)==1 else Path(sys.argv[1])
manifest=root/'data/goldenrod/source-hashes.json'
changes=[name for name,sha in json.loads(manifest.read_text()).items() if not (root/name).is_file() or hashlib.sha256((root/name).read_bytes()).hexdigest()!=sha]
if changes:
 print('Goldenrod native source changed without regenerating its view atlas:',file=sys.stderr)
 for name in changes:print('  '+name,file=sys.stderr)
 print('Run tools/gba/maps/goldenrod_city/stream.py ENGINE from the Pokemon Apocrypha hub, then commit native source and generated atlas together.',file=sys.stderr)
 raise SystemExit(1)
print('Goldenrod native source and viewport atlas agree.')
