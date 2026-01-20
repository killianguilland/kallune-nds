import sys, os, subprocess, struct

def get_png_dims(path):
    with open(path, 'rb') as f:
        f.seek(16)
        return struct.unpack('>II', f.read(8))

def process(png_path, target_output):
    base_name = os.path.basename(png_path).split('.')[0].replace('-', '_')
    build_dir = os.path.abspath(os.path.dirname(target_output))

    grit_cmd = [
        'grit', png_path,
        '-gb', '-gB16', '-ftb', '-p!', '-m!',
        '-o', os.path.join(build_dir, base_name)
    ]
    subprocess.run(grit_cmd, check=True)

    bin_path = os.path.join(build_dir, f"{base_name}.img.bin")
    if not os.path.exists(bin_path):
        bin_path = bin_path.replace('.img.bin', '.bin')

    w, h = get_png_dims(png_path)
    with open(bin_path, "rb") as f:
        data = f.read()
        pixels = list(struct.unpack(f'<{len(data)//2}H', data))

    transparent_color_key = pixels[0]
    # print(f"  [Debug] Couleur de transparence détectée : {hex(transparent_color_key)}")

    for i in range(len(pixels)):
        if pixels[i] == transparent_color_key:
            pixels[i] = 0x0000
        else:
            pixels[i] |= 0x8000 # On force le bit d'Alpha pour être sûr que c'est opaque

    output_data, line_offsets = [], []
    for y in range(h):
        if y < 4:
            output_data.append(0)
            continue
        line_offsets.append(len(output_data))
        row = pixels[y*w : (y+1)*w]
        spans, x = [], 0
        while x < w:
            if x < len(row) and row[x] != 0:
                start_x = x
                curr = []
                while x < w and x < len(row) and row[x] != 0:
                    curr.append(row[x])
                    x += 1
                spans.append((start_x, curr))
            else:
                x += 1
        output_data.append(len(spans))
        for sx, p in spans:
            output_data.extend([sx, len(p), *p])

    cpp_path = os.path.join(build_dir, base_name + ".cpp")
    h_path = os.path.join(build_dir, base_name + ".h")
    
    with open(cpp_path, "w") as f:
        f.write('#include <nds.h>\nextern "C" {\n')
        f.write(f'  extern const u32 {base_name}_offsets[{h}] = {{ {",".join(map(str, line_offsets))} }};\n')
        f.write(f'  extern const u16 {base_name}_data[] = {{ {",".join(map(str, output_data))} }};\n}}\n')

    with open(h_path, "w") as f:
        f.write(f'#pragma once\n#include <nds.h>\nextern "C" {{\n')
        f.write(f'  extern const u32 {base_name}_offsets[{h}];\n')
        f.write(f'  extern const u16 {base_name}_data[];\n}}\n')

if __name__ == "__main__":
    process(sys.argv[1], sys.argv[2])