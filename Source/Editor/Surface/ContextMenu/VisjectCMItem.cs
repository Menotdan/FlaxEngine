// Copyright (c) 2012-2023 Wojciech Figat. All rights reserved.

using System;
using System.Collections.Generic;
using System.Linq;
using FlaxEditor.Surface.Elements;
using FlaxEditor.Utilities;
using FlaxEngine;
using FlaxEngine.GUI;

namespace FlaxEditor.Surface.ContextMenu
{
    /// <summary>
    /// Single item used for <see cref="VisjectCM"/>. Represents type of the <see cref="NodeArchetype"/> that can be spawned.
    /// </summary>
    /// <seealso cref="FlaxEngine.GUI.Control" />
    [HideInEditor]
    public sealed class VisjectCMItem : Control
    {
        private bool _isMouseDown;
        private List<Rectangle> _highlights;
        private GroupArchetype _groupArchetype;
        private NodeArchetype _archetype;
        private bool _isStartsWithMatch;
        private bool _isFullMatch;

        /// <summary>
        /// Gets the item's group
        /// </summary>
        public VisjectCMGroup Group { get; }

        /// <summary>
        /// Gets the group archetype.
        /// </summary>
        public GroupArchetype GroupArchetype => _groupArchetype;

        /// <summary>
        /// Gets the node archetype.
        /// </summary>
        public NodeArchetype NodeArchetype => _archetype;

        /// <summary>
        /// Gets and sets the data of the node.
        /// </summary>
        public object[] Data;

        /// <summary>
        /// A computed score for the context menu order
        /// </summary>
        public float SortScore;

        /// <summary>
        /// Initializes a new instance of the <see cref="VisjectCMItem"/> class.
        /// </summary>
        /// <param name="group">The group.</param>
        /// <param name="groupArchetype">The group archetype.</param>
        /// <param name="archetype">The archetype.</param>
        public VisjectCMItem(VisjectCMGroup group, GroupArchetype groupArchetype, NodeArchetype archetype)
        : base(0, 0, 120, 12)
        {
            Group = group;
            _groupArchetype = groupArchetype;
            _archetype = archetype;
            TooltipText = _archetype.Description;
        }

        /// <summary>
        /// Updates the <see cref="SortScore"/>
        /// </summary>
        /// <param name="selectedBox">The currently user-selected box</param>
        public void UpdateScore(Box selectedBox)
        {
            SortScore = 0;

            if (!(_highlights?.Count > 0))
                return;
            if (!Visible)
                return;

            if (selectedBox != null)
            {
                if (CanConnectTo(selectedBox, NodeArchetype))
                    SortScore += 1;
                if (HasMatchingTypeExactly(selectedBox, NodeArchetype))
                    SortScore += 6;
            }

            if (Data != null)
                SortScore += 1;
            if (_isStartsWithMatch)
                SortScore += 2;
            if (_isFullMatch)
                SortScore += 5;
        }

        private void GetTextRectangle(out Rectangle textRect)
        {
            textRect = new Rectangle(22, 0, Width - 24, Height);
        }

        private bool CanConnectTo(Box startBox, NodeArchetype nodeArchetype)
        {
            if (startBox == null)
                return false;
            if (nodeArchetype.Elements == null)
                return false;

            if (startBox.IsOutput)
            {
                for (int i = 0; i < nodeArchetype.Elements.Length; i++)
                {
                    if (nodeArchetype.Elements[i].Type == NodeElementType.Input &&
                        startBox.CanUseType(nodeArchetype.Elements[i].ConnectionsType))
                    {
                        return true;
                    }
                }
            } else
            {
                for (int i = 0; i < nodeArchetype.Elements.Length; i++)
                {
                    if (nodeArchetype.Elements[i].Type == NodeElementType.Output &&
                        startBox.CanUseType(nodeArchetype.Elements[i].ConnectionsType))
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        private string GetConnectionHintName(Box startBox)
        {
            string flags_display = "";
            int hints = (int)(object)startBox.ParentNode.Archetype.ConnectionsHints;

            if (hints == (int)(object) ConnectionsHint.All)
            {
                return "All";
            }

            if (hints == (int)(object) ConnectionsHint.None)
            {
                return "None";
            }

            if ((hints & (int)(object) ConnectionsHint.Numeric) != 0)
            {
                flags_display += " | Numeric";
            }
            else if ((hints & (int)(object)ConnectionsHint.Scalar) != 0)
            {
                flags_display += " | Scalar";
            }
            else if ((hints & (int)(object)ConnectionsHint.Vector) != 0)
            {
                flags_display += " | Vector";
            }

            if (hints == (int)(object)ConnectionsHint.Anything)
            {
                flags_display += " | Anything";
            }

            if ((hints & (int)(object)ConnectionsHint.Enum) != 0)
            {
                flags_display += " | Enum";
            }

            if ((hints & (int)(object)ConnectionsHint.Array) != 0)
            {
                flags_display += " | Array";
            }

            if ((hints & (int)(object)ConnectionsHint.Dictionary) != 0)
            {
                flags_display += " | Dictionary";
            }

            if ((hints & (int)(object)ConnectionsHint.Value) != 0)
            {
                flags_display += " | Value";
            }

            return flags_display;
        }

        private bool CouldEverConnectTo(Box startBox, NodeArchetype nodeArchetype)
        {
            if (CanConnectTo(startBox, nodeArchetype))
            {
                return true;
            } else
            {
                Debug.Log(String.Format("{0}, {1}", GetConnectionHintName(startBox), nodeArchetype.Title));
            }
            return false;
        }

        private bool HasMatchingTypeExactly(Box startBox, NodeArchetype nodeArchetype)
        {
            if (startBox == null)
                return false;
            if (nodeArchetype.Elements == null)
                return false;

            if (startBox.IsOutput)
            {
                for (int i = 0; i < nodeArchetype.Elements.Length; i++)
                {
                    if (nodeArchetype.Elements[i].Type == NodeElementType.Input &&
                        startBox.Archetype.ConnectionsType == nodeArchetype.Elements[i].ConnectionsType)
                    {
                        return true;
                    }
                }
            }
            else
            {
                for (int i = 0; i < nodeArchetype.Elements.Length; i++)
                {
                    if (nodeArchetype.Elements[i].Type == NodeElementType.Output &&
                        startBox.Archetype.ConnectionsType == nodeArchetype.Elements[i].ConnectionsType)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// Updates the filter.
        /// </summary>
        /// <param name="filterText">The filter text.</param>
        public void UpdateFilter(string filterText)
        {
            _isStartsWithMatch = _isFullMatch = false;
            if (filterText == null)
            {
                // Clear filter
                _highlights?.Clear();
                Visible = true;
            }
            else
            {
                GetTextRectangle(out var textRect);
                if (QueryFilterHelper.Match(filterText, _archetype.Title, out var ranges))
                {
                    // Update highlights
                    if (_highlights == null)
                        _highlights = new List<Rectangle>(ranges.Length);
                    else
                        _highlights.Clear();
                    var style = Style.Current;
                    var font = style.FontSmall;
                    for (int i = 0; i < ranges.Length; i++)
                    {
                        var start = font.GetCharPosition(_archetype.Title, ranges[i].StartIndex);
                        var end = font.GetCharPosition(_archetype.Title, ranges[i].EndIndex);
                        _highlights.Add(new Rectangle(start.X + textRect.X, 0, end.X - start.X, Height));

                        if (ranges[i].StartIndex <= 0)
                        {
                            _isStartsWithMatch = true;
                            if (ranges[i].Length == _archetype.Title.Length)
                                _isFullMatch = true;
                        }
                    }
                    Visible = true;
                }
                else if (_archetype.AlternativeTitles?.Any(altTitle => string.Equals(filterText, altTitle, StringComparison.CurrentCultureIgnoreCase)) == true)
                {
                    // Update highlights
                    if (_highlights == null)
                        _highlights = new List<Rectangle>(1);
                    else
                        _highlights.Clear();
                    var style = Style.Current;
                    var font = style.FontSmall;
                    var start = font.GetCharPosition(_archetype.Title, 0);
                    var end = font.GetCharPosition(_archetype.Title, _archetype.Title.Length - 1);
                    _highlights.Add(new Rectangle(start.X + textRect.X, 0, end.X - start.X, Height));
                    _isFullMatch = true;
                    Visible = true;
                }
                else if (NodeArchetype.TryParseText != null && NodeArchetype.TryParseText(filterText, out var data))
                {
                    // Update highlights
                    if (_highlights == null)
                        _highlights = new List<Rectangle>(1);
                    else
                        _highlights.Clear();
                    var style = Style.Current;
                    var font = style.FontSmall;
                    var start = font.GetCharPosition(_archetype.Title, 0);
                    var end = font.GetCharPosition(_archetype.Title, _archetype.Title.Length - 1);
                    _highlights.Add(new Rectangle(start.X + textRect.X, 0, end.X - start.X, Height));
                    Visible = true;

                    Data = data;
                }
                else
                {
                    // Hide
                    _highlights?.Clear();
                    Visible = false;
                }
            }
        }

        /// <summary>
        /// Updates the item's visibilty based on if it's able to be connected to compareBox.
        /// </summary>
        /// <param name="compareBox">The box to check if we can connect to.</param>
        /// <param name="preserveHidden">Should we preserve previous hidden values.</param>
        public void UpdateCompatibleItem(Box compareBox, bool preserveHidden)
        {
            bool shouldShow = compareBox == null;
            if (shouldShow)
            {
                Visible = true && (!preserveHidden || Visible);
                return;
            }

            shouldShow = CouldEverConnectTo(compareBox, NodeArchetype);
            Visible = shouldShow && (!preserveHidden || Visible);
        }

        /// <inheritdoc />
        public override void Draw()
        {
            var style = Style.Current;
            var rect = new Rectangle(Float2.Zero, Size);
            GetTextRectangle(out var textRect);
            var showScoreHit = SortScore > 0.1f;

            // Overlay
            if (IsMouseOver)
                Render2D.FillRectangle(rect, style.BackgroundHighlighted);

            if (Group.ContextMenu.SelectedItem == this)
                Render2D.FillRectangle(rect, style.BackgroundSelected);

            // Shift text with highlights if using score mark
            if (showScoreHit)
            {
                Render2D.PushTransform(Matrix3x3.Translation2D(12, 0));
            }

            // Draw all highlights
            if (_highlights != null)
            {
                var color = style.ProgressNormal * 0.6f;
                for (int i = 0; i < _highlights.Count; i++)
                    Render2D.FillRectangle(_highlights[i], color);
            }

            // Draw name
            Render2D.DrawText(style.FontSmall, _archetype.Title, textRect, Enabled ? style.Foreground : style.ForegroundDisabled, TextAlignment.Near, TextAlignment.Center);
            if (_archetype.SubTitle != null)
            {
                var titleLength = style.FontSmall.MeasureText(_archetype.Title).X;
                var subTitleRect = new Rectangle(textRect.X + titleLength, textRect.Y, textRect.Width - titleLength, textRect.Height);
                Render2D.DrawText(style.FontSmall, _archetype.SubTitle, subTitleRect, style.ForegroundDisabled, TextAlignment.Near, TextAlignment.Center);
            }

            // Reset transform and draw score mark
            if (showScoreHit)
            {
                Render2D.PopTransform();
                Render2D.DrawText(style.FontSmall, "> ", textRect, Enabled ? style.Foreground : style.ForegroundDisabled, TextAlignment.Near, TextAlignment.Center);
            }
        }

        /// <inheritdoc />
        public override bool OnMouseDown(Float2 location, MouseButton button)
        {
            if (button == MouseButton.Left)
            {
                _isMouseDown = true;
            }

            return base.OnMouseDown(location, button);
        }

        /// <inheritdoc />
        public override bool OnMouseUp(Float2 location, MouseButton button)
        {
            if (button == MouseButton.Left && _isMouseDown)
            {
                _isMouseDown = false;
                Group.ContextMenu.OnClickItem(this);
            }

            return base.OnMouseUp(location, button);
        }

        /// <inheritdoc />
        public override void OnMouseLeave()
        {
            _isMouseDown = false;

            base.OnMouseLeave();
        }

        /// <inheritdoc />
        public override int Compare(Control other)
        {
            if (other is VisjectCMItem otherItem)
            {
                int order = -1 * SortScore.CompareTo(otherItem.SortScore);
                if (order == 0)
                {
                    order = string.Compare(_archetype.Title, otherItem._archetype.Title, StringComparison.Ordinal);
                }
                return order;
            }
            return base.Compare(other);
        }
    }
}
