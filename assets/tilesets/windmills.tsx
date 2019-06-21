<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="windmills" tilewidth="320" tileheight="320" tilecount="4" columns="2">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value="../audio/ambient/windmill/windmill.wav"/>
   <property name="follow_path" type="bool" value="false"/>
   <property name="follow_path_ccw" type="bool" value="false"/>
   <property name="follow_path_period" type="float" value="10"/>
   <property name="follow_path_shape" value=""/>
   <property name="keyframes" type="file" value="windmills/rough/windmill1.json"/>
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
   <property name="spin" type="bool" value="true"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="192" height="192" source="windmills/rough/windmill1.png"/>
  <objectgroup draworder="index">
   <object id="3" x="80" y="0" width="32" height="192">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
   </object>
   <object id="15" x="0" y="80" width="192" height="32">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
   </object>
   <object id="16" x="64" y="64" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="1" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value="../audio/ambient/windmill/windmill.wav"/>
   <property name="follow_path" type="bool" value="false"/>
   <property name="follow_path_ccw" type="bool" value="false"/>
   <property name="follow_path_period" type="float" value="10"/>
   <property name="follow_path_shape" value=""/>
   <property name="keyframes" type="file" value="windmills/rough/windmill2.json"/>
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
   <property name="spin" type="bool" value="true"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="320" height="320" source="windmills/rough/windmill2.png"/>
  <objectgroup draworder="index">
   <object id="1" x="143" y="0" width="32" height="319">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
   </object>
   <object id="2" x="1" y="144" width="318" height="32">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
   </object>
   <object id="3" x="128" y="128" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="hit_sound_1" type="file" value=""/>
     <property name="hit_sound_2" type="file" value=""/>
     <property name="hit_sound_3" type="file" value=""/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <ellipse/>
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
   <property name="keyframes" type="file" value="windmills/rubber/windmill1.json"/>
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
   <property name="spin" type="bool" value="true"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="192" height="192" source="windmills/rubber/windmill1.png"/>
  <objectgroup draworder="index">
   <object id="1" x="80" y="0" width="32" height="192">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="hit_sound_1" type="file" value="../audio/hit/rubber/1.wav"/>
     <property name="hit_sound_2" type="file" value="../audio/hit/rubber/2.wav"/>
     <property name="hit_sound_3" type="file" value="../audio/hit/rubber/3.wav"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="1.5"/>
    </properties>
   </object>
   <object id="2" x="0" y="80" width="190" height="32">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="hit_sound_1" type="file" value="../audio/hit/rubber/1.wav"/>
     <property name="hit_sound_2" type="file" value="../audio/hit/rubber/2.wav"/>
     <property name="hit_sound_3" type="file" value="../audio/hit/rubber/3.wav"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="1.5"/>
    </properties>
   </object>
   <object id="3" x="64" y="64" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="hit_sound_1" type="file" value="../audio/hit/rubber/1.wav"/>
     <property name="hit_sound_2" type="file" value="../audio/hit/rubber/2.wav"/>
     <property name="hit_sound_3" type="file" value="../audio/hit/rubber/3.wav"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="1.5"/>
    </properties>
    <ellipse/>
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
   <property name="keyframes" type="file" value="windmills/rubber/windmill2.json"/>
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
   <property name="spin" type="bool" value="true"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="320" height="320" source="windmills/rubber/windmill2.png"/>
  <objectgroup draworder="index">
   <object id="1" x="144" y="0" width="32" height="318">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="hit_sound_1" type="file" value="../audio/hit/rubber/1.wav"/>
     <property name="hit_sound_2" type="file" value="../audio/hit/rubber/2.wav"/>
     <property name="hit_sound_3" type="file" value="../audio/hit/rubber/3.wav"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="1.5"/>
    </properties>
   </object>
   <object id="2" x="0" y="144" width="318" height="32">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="hit_sound_1" type="file" value="../audio/hit/rubber/1.wav"/>
     <property name="hit_sound_2" type="file" value="../audio/hit/rubber/2.wav"/>
     <property name="hit_sound_3" type="file" value="../audio/hit/rubber/3.wav"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="1.5"/>
    </properties>
   </object>
   <object id="3" x="128" y="128" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="hit_sound_1" type="file" value="../audio/hit/rubber/1.wav"/>
     <property name="hit_sound_2" type="file" value="../audio/hit/rubber/2.wav"/>
     <property name="hit_sound_3" type="file" value="../audio/hit/rubber/3.wav"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="1.5"/>
    </properties>
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
</tileset>
