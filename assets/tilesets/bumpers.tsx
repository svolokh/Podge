<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="bumpers" tilewidth="64" tileheight="64" tilecount="1" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="bumper">
  <properties>
   <property name="bump_force" type="float" value="250"/>
   <property name="follow_path" type="bool" value="false"/>
   <property name="follow_path_ccw" type="bool" value="false"/>
   <property name="follow_path_period" type="float" value="10"/>
   <property name="follow_path_shape" value=""/>
   <property name="keyframes" type="file" value="bumpers/bumper.json"/>
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
  <image width="64" height="64" source="bumpers/bumper.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="hit_sound_1" type="file" value="../audio/hit/rubber/1.wav"/>
     <property name="hit_sound_2" type="file" value="../audio/hit/rubber/2.wav"/>
     <property name="hit_sound_3" type="file" value="../audio/hit/rubber/3.wav"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
</tileset>
