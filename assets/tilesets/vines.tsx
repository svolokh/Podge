<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="vines" tilewidth="640" tileheight="256" tilecount="8" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="follow_path" type="bool" value="false"/>
   <property name="follow_path_ccw" type="bool" value="false"/>
   <property name="follow_path_period" type="float" value="10"/>
   <property name="follow_path_shape" value=""/>
   <property name="keyframes" type="file" value="vines/vine1.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="640" height="256" source="vines/vine1.png"/>
  <objectgroup draworder="index">
   <object id="1" x="637" y="257">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -214,-20 -210,-53 2,-38"/>
   </object>
   <object id="2" x="428" y="203">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -101,-15 -174,-54 -210,-106 -239,-88 -196,-27 -111,16 -5,33"/>
   </object>
   <object id="3" x="220" y="98">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -7,-23 -10,-52 -22,-63 -41,-59 -44,-28 -31,16"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="1" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="follow_path" type="bool" value="false"/>
   <property name="follow_path_ccw" type="bool" value="false"/>
   <property name="follow_path_period" type="float" value="10"/>
   <property name="follow_path_shape" value=""/>
   <property name="keyframes" type="file" value="vines/vine2.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="432" height="91" source="vines/vine2.png"/>
  <objectgroup draworder="index">
   <object id="1" x="32" y="30">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -17,-3 -29,12 -24,28 17,44 35,14"/>
   </object>
   <object id="2" x="68" y="45">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 34,2 48,36 9,37 -17,30"/>
   </object>
   <object id="3" x="102" y="48">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 55,-18 77,17 13,32"/>
   </object>
   <object id="4" x="158" y="33">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 52,-18 76,14 20,32"/>
   </object>
   <object id="5" x="211" y="15">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 45,-10 72,27 38,27 19,34"/>
   </object>
   <object id="6" x="258" y="7">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 26,-2 64,3 66,38 26,34"/>
   </object>
   <object id="7" x="323" y="10">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 109,17 110,54 0,35"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="2" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="follow_path" type="bool" value="false"/>
   <property name="follow_path_ccw" type="bool" value="false"/>
   <property name="follow_path_period" type="float" value="10"/>
   <property name="follow_path_shape" value=""/>
   <property name="keyframes" type="file" value="vines/vine3.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="230" height="256" source="vines/vine3.png"/>
  <objectgroup draworder="index">
   <object id="1" x="230" y="208">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -61,-4 -73,26 -36,32 4,35"/>
   </object>
   <object id="2" x="169" y="204">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -45,-20 -67,9 -10,32"/>
   </object>
   <object id="3" x="123" y="186">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -27,-22 -58,-6 -43,13 -18,29"/>
   </object>
   <object id="4" x="95" y="166">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -17,-41 -51,-32 -31,14"/>
   </object>
   <object id="6" x="77" y="125">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -7,-30 -42,-20 -33,9"/>
   </object>
   <object id="7" x="72" y="95">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -13,-54 -29,-62 -44,-50 -36,11"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="3" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="follow_path" type="bool" value="false"/>
   <property name="follow_path_ccw" type="bool" value="false"/>
   <property name="follow_path_period" type="float" value="10"/>
   <property name="follow_path_shape" value=""/>
   <property name="keyframes" type="file" value="vines/vine4.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="128" height="256" source="vines/vine4.png"/>
  <objectgroup draworder="index">
   <object id="1" x="47" y="8">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 12,-7 27,-7 34,4 35,38 1,37"/>
   </object>
   <object id="2" x="83" y="44">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -3,186 -38,185 -35,1"/>
   </object>
   <object id="3" x="81" y="229">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -1,10 -17,19 -31,15 -35,0"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="4" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="follow_path" type="bool" value="false"/>
   <property name="follow_path_ccw" type="bool" value="false"/>
   <property name="follow_path_period" type="float" value="10"/>
   <property name="follow_path_shape" value=""/>
   <property name="keyframes" type="file" value="vines/vine5.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="640" height="256" source="vines/vine5.png"/>
  <objectgroup draworder="index">
   <object id="1" x="213" y="49">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 2,18 8,37 -33,52 -39,35 -40,9 -35,-13 -17,-15"/>
   </object>
   <object id="2" x="222" y="86">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 22,37 -21,61 -42,16"/>
   </object>
   <object id="3" x="245" y="123">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 21,28 -10,60 -44,25"/>
   </object>
   <object id="4" x="267" y="151">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 39,18 12,56 -31,32"/>
   </object>
   <object id="5" x="307" y="170">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 31,11 7,54 -28,37"/>
   </object>
   <object id="7" x="340" y="181">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 33,7 10,52 -26,43"/>
   </object>
   <object id="8" x="373" y="189">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 267,26 267,67 184,64 -23,43"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="5" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="follow_path" type="bool" value="false"/>
   <property name="follow_path_ccw" type="bool" value="false"/>
   <property name="follow_path_period" type="float" value="10"/>
   <property name="follow_path_shape" value=""/>
   <property name="keyframes" type="file" value="vines/vine6.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="432" height="91" source="vines/vine6.png"/>
  <objectgroup draworder="index">
   <object id="1" x="432" y="23">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -57,-9 -127,-17 -128,22 -68,32 0,42"/>
   </object>
   <object id="2" x="305" y="6">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -48,0 -96,9 -82,50 -43,38 0,39"/>
   </object>
   <object id="3" x="210" y="15">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -100,27 -89,70 14,43"/>
   </object>
   <object id="4" x="111" y="40">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -33,0 -51,40 -17,47 11,45"/>
   </object>
   <object id="5" x="78" y="40">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -37,-15 -59,-15 -70,-2 -67,23 -17,41"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="6" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="follow_path" type="bool" value="false"/>
   <property name="follow_path_ccw" type="bool" value="false"/>
   <property name="follow_path_period" type="float" value="10"/>
   <property name="follow_path_shape" value=""/>
   <property name="keyframes" type="file" value="vines/vine8.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="128" height="256" source="vines/vine8.png"/>
  <objectgroup draworder="index">
   <object id="2" x="64" y="-3">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 21,7 21,87 -24,88 -25,10"/>
   </object>
   <object id="3" x="86" y="83">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 0,86 0,133 -49,133 -48,73 -47,1"/>
   </object>
   <object id="4" x="87" y="216">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 -1,29 -24,39 -51,29 -50,1"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="7" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="follow_path" type="bool" value="false"/>
   <property name="follow_path_ccw" type="bool" value="false"/>
   <property name="follow_path_period" type="float" value="10"/>
   <property name="follow_path_shape" value=""/>
   <property name="keyframes" type="file" value="vines/vine9.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="230" height="256" source="vines/vine9.png"/>
  <objectgroup draworder="index">
   <object id="1" x="66" y="39">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 5,28 15,70 -34,82 -50,9 -34,-14 -11,-12"/>
   </object>
   <object id="2" x="83" y="109">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 16,45 -38,64 -51,13"/>
   </object>
   <object id="3" x="100" y="154">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 8,17 37,30 0,65 -43,40 -54,21"/>
   </object>
   <object id="4" x="139" y="184">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 40,13 4,56 -38,35"/>
   </object>
   <object id="5" x="179" y="196">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <polygon points="0,0 51,3 51,53 -35,44"/>
   </object>
  </objectgroup>
 </tile>
</tileset>
