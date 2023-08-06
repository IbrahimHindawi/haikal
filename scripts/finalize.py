import os

name = os.getlogin()
vcpkg_user_props = "C:\\Users\\" + name + "\\AppData\\Local\\vcpkg\\vcpkg.user.props"
# print(vcpkg_user_props)
f = open(vcpkg_user_props)
lines = f.readlines()
f.close()

import_line = lines[2]

open_paren_loc = 0
for char in import_line:
    if char == '(':
        break
    open_paren_loc += 1

closed_paren_loc = 0
for char in import_line:
    if char == ')':
        break
    closed_paren_loc += 1

# # print(open_paren_loc)
# # print(closed_paren_loc)

# print(import_line[open_paren_loc + 1:closed_paren_loc])
path = import_line[open_paren_loc + 1:closed_paren_loc]

path_chunks = path.split('\\')
# print(path_chunks)

vcpkg_loc = 0
for chunk in path_chunks:
    if chunk == 'vcpkg':
        break
    vcpkg_loc += 1
vcpkg_loc += 1

vcpkg_path = '/'.join(path_chunks[0:vcpkg_loc])
# print(vcpkg_path)
vcpkg_include_path = vcpkg_path[1:] + '/installed/x64-windows/include'
# print(vcpkg_include_path)

vcpkg_include_path_cmd = ' -I\\"' + vcpkg_include_path + '\\"'
# print(vcpkg_include_path_cmd)

f = open("compile_commands.json")
lines = f.readlines()
f.close()

json_payload = []
for line in lines:
    # # print(line)
    if "command" in line:
        # # print(line)
        end_of_line_location = len(line) - 1
        cut = end_of_line_location - 2
        head = line[0:cut]
        tail = line[cut:]
        new_line = head + vcpkg_include_path_cmd + tail
        # print(new_line)
        json_payload.append(new_line)
        continue
    json_payload.append(line)

# # print(json_payload)
json = ''.join(json_payload)
# print(json)

f = open('compile_commands.json', 'w')
f.write(json)
f.close()
print('compile_commands.json finalized')
