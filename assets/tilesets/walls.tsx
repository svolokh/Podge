<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="walls" tilewidth="64" tileheight="64" tilecount="7" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="keyframes" type="file" value=""/>
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
  <image width="64" height="64" source="wall1.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
 <tile id="1" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="keyframes" type="file" value="wall2.json"/>
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
  <image width="64" height="64" source="wall2.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 32,32 64,0"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="2" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="keyframes" type="file" value="wall3.json"/>
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
  <image width="64" height="64" source="wall3.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 32,-32 64,0"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="3" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="keyframes" type="file" value="wall4.json"/>
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
  <image width="64" height="64" source="wall4.png"/>
  <objectgroup draworder="index">
   <object id="2" x="0" y="0">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 64,64 0,64"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="4" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="keyframes" type="file" value="wall5.json"/>
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
  <image width="64" height="64" source="wall5.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 64,-64 64,0"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="5" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="keyframes" type="file" value="wall6.json"/>
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
  <image width="64" height="64" source="wall6.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 64,-64 0,-64"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="6" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" type="file" value=""/>
   <property name="keyframes" type="file" value="wall7.json"/>
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
  <image width="64" height="64" source="wall7.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 64,64 64,0"/>
   </object>
  </objectgroup>
 </tile>
</tileset>
