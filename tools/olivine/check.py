"""Fail a production build if native Olivine edits have a stale view atlas."""
from pathlib import Path
import hashlib,json,sys
root=Path(__file__).resolve().parents[2] if len(sys.argv)==1 else Path(sys.argv[1])
manifest=root/'data/olivine/source-hashes.json'
def digest(p):
 data=p.read_bytes()
 if p.suffix=='.pal':data=data.replace(b'\r\n',b'\n')
 return hashlib.sha256(data).hexdigest()
changes=[name for name,sha in json.loads(manifest.read_text()).items() if not (root/name).is_file() or digest(root/name)!=sha]
if changes:
 print('Olivine native source changed without regenerating its view atlas:',file=sys.stderr)
 for name in changes:print('  '+name,file=sys.stderr)
 print('Run tools/gba/maps/olivine_city/stream.py ENGINE from the Pokemon Apocrypha hub, then commit native source and generated atlas together.',file=sys.stderr)
 raise SystemExit(1)
print('Olivine native source and viewport atlas agree.')
